#pragma once

struct model_render_t
{
    auto force_material_override(material_t* mat)
    {
        return memory::get_vfunc<void (*)(void*, material_t*, int, int)>(this, 1)(this, mat, 0, 0);
    }
};