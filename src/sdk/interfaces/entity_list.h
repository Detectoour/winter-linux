#pragma once

struct entity_list_t
{
    auto get_client_entity(int idx)
    {
        return memory::get_vfunc<void* (*)(void*, int)>(this, 3)(this, idx);
    }

    auto get_client_entity_from_handle(std::size_t handle)
    {
        return memory::get_vfunc<void* (*)(void*, std::size_t)>(this, 4)(this, handle);
    }

    auto get_highest_entity_index()
    {
        return memory::get_vfunc<int (*)(void*)>(this, 6)(this);
    }

    template <class t>
    auto get(int entity_number)
    {
        return static_cast<t>(get_client_entity(entity_number));
    }

    template <class t>
    auto get(std::size_t entity)
    {
        return static_cast<t>(get_client_entity_from_handle(entity));
    }
};