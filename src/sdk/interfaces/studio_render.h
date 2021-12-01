#pragma once

struct studio_render_t
{
    char pad_0[600];
    material_t* mat_overrie;
    char pad_1[24];
    int type;

    auto set_color(const color_t& clr)
    {
        float array[4] = { static_cast<float>(clr.r) / 255.f, static_cast<float>(clr.g) / 255.f, static_cast<float>(clr.b) / 255.f, static_cast<float>(clr.a) / 255.f };
        return memory::get_vfunc<void (*)(void*, float*)>(this, 27)(this, array);
    }

    auto set_alpha(float alpha)
    {
        return memory::get_vfunc<void (*)(void*, float)>(this, 28)(this, alpha);
    }

    auto forced_material_override(material_t* mat, int type = 0, int overrides = 0)
    {
        return memory::get_vfunc<void (*)(void*, material_t*, int, int)>(this, 33)(this, mat, type, overrides);
    }

    auto is_overried() const
    {
        if (!mat_overrie)
            return type == 2 || type == 4;
        return std::string_view { mat_overrie->get_name() }.starts_with("dev/glow");
    }
};