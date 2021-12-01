#pragma once

struct render_view_t
{
    auto set_alpha(float alpha)
    {
        return memory::get_vfunc<void (*)(void*, float)>(this, 4)(this, alpha);
    }
    auto get_alpha()
    {
        return memory::get_vfunc<float (*)(void*)>(this, 5)(this);
    }
    auto set_color(const color_t& clr)
    {
        float array[4] = { static_cast<float>(clr.r) / 255.f, static_cast<float>(clr.g) / 255.f, static_cast<float>(clr.b) / 255.f, static_cast<float>(clr.a) / 255.f };

        return memory::get_vfunc<void (*)(void*, float*)>(this, 6)(this, array);
    }
    auto get_color(float* clr)
    {
        return memory::get_vfunc<void (*)(void*, float*)>(this, 7)(this, clr);
    }
};