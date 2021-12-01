#pragma once

struct base_client_t
{
    client_class* get_classes()
    {
        return memory::get_vfunc<client_class* (*)(void*)>(this, 8)(this);
    }
};