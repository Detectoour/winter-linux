#pragma once

#include <immintrin.h>
#include <vector>
#include <cstdint>
#include <array>

namespace xklib
{
    class pattern_bytes
    {
    public:
        class value
        {
        public:
            value(int value);

            enum
            {
                INVALID = -1,
                UNKNOWN = -2
            };

            int _value = INVALID;
            std::size_t _index {};
        };

#ifdef __AVX512F__
        using simd_value_t = __m512i;
#elif defined(__AVX2__)
        using simd_value_t = __m256i;
#else
        using simd_value_t = uint64_t;
#endif
        struct simd_unknown_value_t
        {
            std::size_t simd_index;
            std::size_t simd_byte_index;
            std::size_t data_byte_index;
            std::size_t size_to_copy;
        };

        struct simd_mv_t
        {
            simd_value_t mask;
            simd_value_t value;
        };

        struct organized_values_t
        {
            std::vector<uint8_t> bytes;
            std::size_t skip_bytes;
        };

        pattern_bytes();
        pattern_bytes(const std::vector<value> values);

    public:
        bool is_valid();
        std::vector<simd_mv_t>& fast_aligned_mvs();
        std::vector<value>& bytes();
        std::vector<organized_values_t>& vec_organized_values();

    private:
        std::vector<value> _bytes;
        std::vector<simd_unknown_value_t> _unknown_values;
        std::vector<organized_values_t> _vec_organized_values;
        std::vector<simd_mv_t> _fast_aligned_mvs;
    };

    class pattern_scanning
    {
    public:
        static uintptr_t search_v1(pattern_bytes& pattern, uint8_t* data, std::size_t size, uintptr_t base_address);
        static uintptr_t search_v1_aligned(pattern_bytes& pattern, uint8_t* data, std::size_t size, uintptr_t base_address);
        static uintptr_t search_v2(pattern_bytes& pattern, uint8_t* data, std::size_t size, uintptr_t base_address);
    };
}

struct pattern_info_t
{
    pattern_info_t()
    {
    }

    template <std::size_t N>
    pattern_info_t(const std::array<int, N>& bytes)
    {
        std::vector<xklib::pattern_bytes::value> pattern { bytes.begin(), bytes.end() };
        this->arry_bytes       = { bytes.begin(), bytes.end() };
        this->bytes            = xklib::pattern_bytes(pattern);
        this->offset           = 0;
        this->aligned_method   = true;
        this->relative_address = false;
    }

    template <std::size_t N>
    pattern_info_t(const std::array<int, N>& bytes, std::size_t offset, bool aligned_method = true, bool relative = false)
    {
        std::vector<xklib::pattern_bytes::value> pattern { bytes.begin(), bytes.end() };
        this->arry_bytes       = { bytes.begin(), bytes.end() };
        this->bytes            = xklib::pattern_bytes(pattern);
        this->offset           = offset;
        this->aligned_method   = aligned_method;
        this->relative_address = relative;
    }

    std::vector<int> arry_bytes;
    xklib::pattern_bytes bytes;
    std::size_t offset;
    bool aligned_method;
    bool relative_address;
};