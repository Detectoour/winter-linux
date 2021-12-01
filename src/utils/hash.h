#pragma once

#include <cstdint>
#include <cstring>

using hash_t = uint32_t;

namespace hashing
{
    constexpr uint64_t BASIS = 0x811c9dc5;
    constexpr uint64_t PRIME = 0x1000193;

    inline hash_t get(const char* txt)
    {
        hash_t ret = BASIS;

        hash_t length = strlen(txt);
        for (auto i = 0u; i < length; ++i)
        {
            ret ^= txt[i];
            ret *= PRIME;
        }

        return ret;
    }
}

#define HASH(str) hashing::get(str)