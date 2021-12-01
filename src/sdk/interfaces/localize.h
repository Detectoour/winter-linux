#pragma once

struct localize_t
{
    auto find(const char* token)
    {
        return memory::get_vfunc<const char* (*)(void*, const char*)>(this, 47)(this, token);
    }
};