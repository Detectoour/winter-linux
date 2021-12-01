#pragma once

struct model_info_t
{
    auto get_model(int idx)
    {
        return memory::get_vfunc<model_t* (*)(void*, int)>(this, 2)(this, idx);
    }
    auto get_model_index(const char* file)
    {
        return memory::get_vfunc<int (*)(void*, const char*)>(this, 3)(this, file);
    }
    auto get_model_name(const model_t* model)
    {
        return memory::get_vfunc<const char* (*)(void*, const model_t*)>(this, 4)(this, model);
    }
    auto get_studio_model(const model_t* model)
    {
        return memory::get_vfunc<studio_hdr_t* (*)(void*, const model_t*)>(this, 31)(this, model);
    }
};