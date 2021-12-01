#pragma once
#include <chrono>
#include <vector>
#include "pattern_scanning.h"

namespace modules
{
    struct library_t
    {
        struct region_t
        {
            uintptr_t address;
            uint8_t* data;
            std::size_t size;
        };

        library_t();

        std::string name;
        std::vector<region_t> regions;

        library_t(std::string mod_name, std::vector<region_t> mod_regions);

        address_t find_pattern(pattern_info_t& info)
        {
            auto _start      = std::chrono::high_resolution_clock::now();
            uintptr_t result = 0;

            for (auto&& region : regions)
            {
                result = info.aligned_method ? xklib::pattern_scanning::search_v1_aligned(info.bytes, region.data, region.size, region.address) :
                                               xklib::pattern_scanning::search_v1(info.bytes, region.data, region.size, region.address);

                if (result)
                    break;
            }

            if (!result)
                return address_t();

            auto _end        = std::chrono::high_resolution_clock::now();
            auto _difference = std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count();

            auto final_address = address_t(result + info.offset);
            if (info.relative_address)
                final_address = final_address.relative();

            logger::log(fmt::format("time: {:.3f}ms, address: {:#09X} offset: {}+{:#09X} result_offset: {:#09X}",
                                    float(_difference) / 1000,
                                    final_address.m_ptr,
                                    this->name,
                                    final_address.m_ptr - regions[0].address,
                                    result));

            return final_address;
        }
    };

    inline library_t client;
    inline library_t engine;
    inline library_t matsurface;
    inline library_t studio_render;
    inline library_t material_system;
    inline library_t file_system;
}

namespace memory
{
    void initialize_modules();

    template <typename t = address_t>
    t get_vfunc(address_t pointer, std::size_t index)
    {
        return static_cast<t>(pointer.get<t*>()[index]);
    }

    inline bool is_valid_address(const address_t& addr)
    {
        return addr != nullptr && addr >= 0x10000 && addr < 0x00007FFFFFFEFFFF;
    }
} // namespace memory