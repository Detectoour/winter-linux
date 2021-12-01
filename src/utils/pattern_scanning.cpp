#include "pattern_scanning.h"
#include "../winter.h"

// Credit goes to XutaxKamay, he made this excellent library, make sure to check it out
// https://github.com/XutaxKamay/XKLib/blob/master/src/patternscanning.cpp
namespace xklib
{
    pattern_bytes::value::value(int value) : _value(value)
    {
    }

    pattern_bytes::pattern_bytes()
    {
    }

    pattern_bytes::pattern_bytes(std::vector<value> values) : _bytes(std::move(values))
    {
        if (!is_valid())
        {
            logger::log("Invalid pattern", log_level::error);
            throw std::runtime_error("Invalid pattern");
            return;
        }

        _unknown_values.reserve(_bytes.size());

        /* index of the byte in the pattern */
        std::size_t index = 0;
        /* count unknown bytes that were contigous inside the pattern */
        std::size_t ukval_contigous_count = 0;
        /**
         * store the last time the unknown bytes were contigous for
         * pushing into a vector the unknown bytes
         */
        std::size_t index_since_contigous_count = 0;
        /* index of the simd value */
        std::size_t byte_simd_index = 0;
        /* count how many unknown bytes we got */
        std::size_t count_unknown_byte = 0;
        /* organized values */
        bool are_known_values = true;
        std::vector<uint8_t> known_bytes;
        simd_value_t simd_value {}, simd_mask {};

        for (auto&& byte : _bytes)
        {
            byte._index = index;
            if (byte._value == value::UNKNOWN)
            {
                index_since_contigous_count = index;
                ukval_contigous_count++;

                if (ukval_contigous_count >= sizeof(simd_value_t))
                {
                    /**
                     * store info about what index would be the simd
                     * value, etc ... */
                    _unknown_values.push_back(
                      { index_since_contigous_count / sizeof(simd_value_t), index_since_contigous_count % sizeof(simd_value_t), index_since_contigous_count, ukval_contigous_count });
                    ukval_contigous_count = 0;
                }

                if (are_known_values)
                {
                    are_known_values = false;
                }

                count_unknown_byte++;
            }
            else
            {
                if (!are_known_values)
                {
                    /* push back the last count of unknown_bytes */
                    _vec_organized_values.push_back({ known_bytes, count_unknown_byte });
                    count_unknown_byte = 0;
                    known_bytes.clear();
                    are_known_values = true;
                }

                /* push back known value */
                known_bytes.push_back(static_cast<uint8_t>(byte._value));

                /* check if previous unknown bytes weren't a full simd
                 * value and push it back */
                if (ukval_contigous_count > 0)
                {
                    _unknown_values.push_back(
                      { index_since_contigous_count / sizeof(simd_value_t), index_since_contigous_count % sizeof(simd_value_t), index_since_contigous_count, ukval_contigous_count });
                    ukval_contigous_count = 0;
                }

                /**
                 * copy pattern data to the simd value and create the
                 * mask
                 */
                reinterpret_cast<uint8_t*>(&simd_value)[byte_simd_index] = static_cast<uint8_t>(byte._value);
                reinterpret_cast<uint8_t*>(&simd_mask)[byte_simd_index]  = 0xFF;
            }

            index++;
            byte_simd_index++;

            /* do wo we got a full simd value here */
            if (byte_simd_index >= sizeof(simd_value_t))
            {
                _fast_aligned_mvs.push_back({ simd_mask, simd_value });

                /* reset values */
                std::memset(&simd_value, 0, sizeof(simd_value));
                std::memset(&simd_mask, 0, sizeof(simd_mask));
                byte_simd_index = 0;
            }
        }
        /* was there still some known values left after an unknown byte */
        if (known_bytes.size())
        {
            /* if yes push it back */
            _vec_organized_values.push_back({ known_bytes, 0 });
        }

        /**
         * was there some left bytes that needs to be stored a simd value
         */
        if (byte_simd_index > 0)
        {
            _fast_aligned_mvs.push_back({ simd_mask, simd_value });
        }
    }

    std::vector<pattern_bytes::value>& pattern_bytes::bytes()
    {
        return _bytes;
    }

    std::vector<pattern_bytes::simd_mv_t>& pattern_bytes::fast_aligned_mvs()
    {
        return _fast_aligned_mvs;
    }

    std::vector<pattern_bytes::organized_values_t>& pattern_bytes::vec_organized_values()
    {
        return _vec_organized_values;
    }

    bool pattern_bytes::is_valid()
    {
        if (_bytes.size() == 0)
        {
            return false;
        }

        for (auto&& byte : _bytes)
        {
            if (byte._value == value::INVALID)
            {
                return false;
            }
        }

        /* xx xx ? */
        if (_bytes[_bytes.size() - 1]._value == value::UNKNOWN)
        {
            return false;
        }

        return true;
    }

    uintptr_t pattern_scanning::search_v1(pattern_bytes& pattern, uint8_t* data, std::size_t size, uintptr_t base_address)
    {
        auto&& pattern_bytes    = pattern.bytes();
        auto pattern_bytes_size = pattern_bytes.size();
        auto&& fast_aligned_mvs = pattern.fast_aligned_mvs();

        uint8_t* start_data   = data;
        uint8_t* current_data = data;

        do
        {
            for (auto&& mv : fast_aligned_mvs)
            {
                /* if ((value & mask) == pattern_value) */
#if defined(__AVX512F__)
                if (!_mm512_cmpeq_epi64_mask(_mm512_and_si512(_mm512_loadu_si512(reinterpret_cast<pattern_bytes::simd_value_t*>(current_data)), _mm512_load_si512(&mv.mask)),
                                             _mm512_load_si512(&mv.value)))
                {
                    goto skip_2;
                }
#elif defined(__AVX2__)
                if (_mm256_movemask_epi8(_mm256_cmpeq_epi64(_mm256_and_si256(_mm256_loadu_si256(reinterpret_cast<pattern_bytes::simd_value_t*>(current_data)), _mm256_load_si256(&mv.mask)),
                                                            _mm256_load_si256(&mv.value)))
                    != -1)
                {
                    goto skip_2;
                }
#else
                if ((*reinterpret_cast<pattern_bytes::simd_value_t*>(current_data) & mv.mask) != mv.value)
                {
                    goto skip_2;
                }
#endif
                current_data += sizeof(pattern_bytes::simd_value_t);
            }

            return base_address + static_cast<uintptr_t>(start_data - data);
        skip_2:
            start_data++;
            current_data = start_data;
        }
        while (current_data + pattern_bytes_size <= data + size);

        return 0;
    }

    uintptr_t pattern_scanning::search_v1_aligned(pattern_bytes& pattern, uint8_t* data, std::size_t size, uintptr_t base_address)
    {
        auto&& pattern_bytes    = pattern.bytes();
        auto pattern_bytes_size = pattern_bytes.size();
        auto&& fast_aligned_mvs = pattern.fast_aligned_mvs();

        uint8_t* start_data   = data;
        uint8_t* current_data = data;

        do
        {
            for (auto&& mv : fast_aligned_mvs)
            {
                /* if ((value & mask) == pattern_value) */
#if defined(__AVX512F__)
                if (!_mm512_cmpeq_epi64_mask(_mm512_and_si512(_mm512_load_si512(reinterpret_cast<pattern_bytes::simd_value_t*>(current_data)), _mm512_load_si512(&mv.mask)),
                                             _mm512_load_si512(&mv.value)))
                {
                    goto skip_2;
                }
#elif defined(__AVX2__)
                if (_mm256_movemask_epi8(_mm256_cmpeq_epi64(_mm256_and_si256(_mm256_load_si256(reinterpret_cast<pattern_bytes::simd_value_t*>(current_data)), _mm256_load_si256(&mv.mask)),
                                                            _mm256_load_si256(&mv.value)))
                    != -1)
                {
                    goto skip_2;
                }
#else
                if ((*reinterpret_cast<pattern_bytes::simd_value_t*>(current_data) & mv.mask) != mv.value)
                {
                    goto skip_2;
                }
#endif
                current_data += sizeof(pattern_bytes::simd_value_t);
            }

            return base_address + static_cast<uintptr_t>(start_data - data);
        skip_2:
            start_data += sizeof(pattern_bytes::simd_value_t);
            current_data = start_data;
        }
        while (current_data + pattern_bytes_size <= data + size);

        return 0;
    }

    uintptr_t pattern_scanning::search_v2(pattern_bytes& pattern, uint8_t* data, std::size_t size, uintptr_t base_address)
    {
        auto&& pattern_bytes        = pattern.bytes();
        auto pattern_size           = pattern_bytes.size();
        auto&& vec_organized_values = pattern.vec_organized_values();

        uint8_t* start_data   = data;
        uint8_t* current_data = data;

        do
        {
            for (auto&& organized_values : vec_organized_values)
            {
                for (auto&& byte : organized_values.bytes)
                {
                    if (byte != *current_data)
                    {
                        goto skip;
                    }

                    current_data++;
                }

                current_data += organized_values.skip_bytes;
            }

            return base_address + static_cast<uintptr_t>(start_data - data);

        skip:
            start_data += sizeof(pattern_bytes::simd_value_t);
            current_data = start_data;
        }
        while ((current_data + pattern_size) <= (data + size));

        return 0;
    }
}