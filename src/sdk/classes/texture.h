#pragma once

struct texture_t
{
    auto get_actual_width()
    {
        return memory::get_vfunc<int (*)(void*)>(this, 3)(this);
    }
    auto get_actual_height()
    {
        return memory::get_vfunc<int (*)(void*)>(this, 4)(this);
    }
    auto increment_reference_count()
    {
        return memory::get_vfunc<void (*)(void*)>(this, 10)(this);
    }
};