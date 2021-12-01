#pragma once

struct debug_overlay_t
{
    auto screen_position(const vec3_t& in, vec3_t& out)
    {
        return memory::get_vfunc<bool (*)(void*, const vec3_t&, vec3_t&)>(this, 11)(this, in, out);
    }
    auto world_to_screen(const vec3_t& point, vec3_t& screen)
    {
        return screen_position(point, screen) != 1;
    }
};