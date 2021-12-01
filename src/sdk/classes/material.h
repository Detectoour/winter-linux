#pragma once

struct material_var_t
{
    auto set_float(float v)
    {
        return memory::get_vfunc<void (*)(void*, float)>(this, 3)(this, v);
    }
};

struct material_t
{
    auto get_name()
    {
        return memory::get_vfunc<const char* (*)(void*)>(this, 0)(this);
    }

    auto get_texture_group_name()
    {
        return memory::get_vfunc<const char* (*)(void*)>(this, 1)(this);
    }

    auto find_var(const char* name, bool* found = nullptr, bool complain = true)
    {
        return memory::get_vfunc<material_var_t* (*)(void*, const char*, bool*, bool)>(this, 11)(this, name, found, complain);
    }

    auto increment_reference_count()
    {
        return memory::get_vfunc<void (*)(void*)>(this, 12)(this);
    }

    auto set_alpha(float alpha)
    {
        return memory::get_vfunc<void (*)(void*, float)>(this, 27)(this, alpha);
    }

    auto set_color(const color_t& clr)
    {
        float array[4] = { static_cast<float>(clr.r) / 255.f, static_cast<float>(clr.g) / 255.f, static_cast<float>(clr.b) / 255.f, static_cast<float>(clr.a) / 255.f };
        return memory::get_vfunc<void (*)(void*, float*)>(this, 28)(this, array);
    }

    auto set_flag(material_var_flags_t flag, bool on)
    {
        return memory::get_vfunc<void (*)(void*, material_var_flags_t, bool)>(this, 29)(this, flag, on);
    }

    auto get_alpha()
    {
        return memory::get_vfunc<float (*)(void*)>(this, 44)(this);
    }

    auto get_color(float* r, float* g, float* b)
    {
        return memory::get_vfunc<void (*)(void*, float*, float*, float*)>(this, 45)(this, r, g, b);
    }
};