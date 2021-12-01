#pragma once

struct static_prop_t
{
    char pad_0[76];
    uint8_t alpha;
    char pad_1[194];
    vec4_t color;
};

struct static_prop_manager_t
{
    auto get_static_props()
    {
        return *reinterpret_cast<utl_vector<static_prop_t>*>(reinterpret_cast<uintptr_t>(this) + 72);
    }
};