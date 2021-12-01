#pragma once

typedef unsigned short material_handle_t;
struct shader_stencil_state_t
{
    bool enable;
    shader_stencil_op_t fail_op;
    shader_stencil_op_t z_fail_op;
    shader_stencil_op_t pass_op;
    shader_stencil_func_t compare_func;
    int reference_value;
    uint32_t test_mask;
    uint32_t write_mask;

    shader_stencil_state_t()
    {
        enable  = false;
        pass_op = fail_op = z_fail_op = shader_stencil_op_t::SHADER_STENCILOP_KEEP;
        compare_func                  = shader_stencil_func_t::SHADER_STENCILFUNC_ALWAYS;
        reference_value               = 0;
        test_mask = write_mask = 0xFFFFFFFF;
    }
};

struct material_render_context_t
{
    auto set_render_target(texture_t* texture)
    {
        return memory::get_vfunc<void (*)(void*, texture_t*)>(this, 6)(this, texture);
    }
    auto clear_buffers(bool a1, bool a2, bool a3)
    {
        return memory::get_vfunc<void (*)(void*, bool, bool, bool)>(this, 12)(this, a1, a2, a3);
    }
    auto view_port(int x, int y, int w, int h)
    {
        return memory::get_vfunc<void (*)(void*, int, int, int, int)>(this, 40)(this, x, y, w, h);
    }
    auto get_view_port(int& x, int& y, int& w, int& h)
    {
        return memory::get_vfunc<void (*)(void*, int&, int&, int&, int&)>(this, 41)(this, x, y, w, h);
    }
    auto clear_3ub(uint8_t r, uint8_t g, uint8_t b)
    {
        return memory::get_vfunc<void (*)(void*, uint8_t, uint8_t, uint8_t)>(this, 78)(this, r, g, b);
    }
    auto override_depth_enable(bool enabled, bool depth_write_enable, bool depth_test_enable = true)
    {
        return memory::get_vfunc<void (*)(void*, bool, bool, bool)>(this, 80)(this, enabled, depth_write_enable, depth_test_enable);
    }
    auto draw_screen_space_rectangle(material_t* mat,
                                     int dest_x,
                                     int dest_y,
                                     int width,
                                     int height,
                                     float src_tex_x0,
                                     float src_tex_y0,
                                     float src_tex_x1,
                                     float src_tex_y1,
                                     int src_tex_width,
                                     int src_tex_height,
                                     void* renderable = nullptr,
                                     int dice_x       = 1,
                                     int dice_y       = 1)
    {
        return memory::get_vfunc<void (*)(void*, material_t*, int, int, int, int, float, float, float, float, int, int, void*, int, int)>(
          this,
          113)(this, mat, dest_x, dest_y, width, height, src_tex_x0, src_tex_y0, src_tex_x1, src_tex_y1, src_tex_width, src_tex_height, renderable, dice_x, dice_y);
    }
    auto push_render_target_and_viewport()
    {
        return memory::get_vfunc<void (*)(void*)>(this, 115)(this);
    }
    auto pop_render_target_and_viewport()
    {
        return memory::get_vfunc<void (*)(void*)>(this, 119)(this);
    }
    auto set_stencil_state(const shader_stencil_state_t& s)
    {
        return memory::get_vfunc<void (*)(void*, const shader_stencil_state_t&)>(this, 127)(this, s);
    }
    auto begin_pix_event(unsigned long clr, const char* name)
    {
        return memory::get_vfunc<void (*)(void*, unsigned long, const char*)>(this, 143)(this, clr, name);
    }
};

struct material_system_t
{
    auto create_material(const char* name, void* kv)
    {
        return memory::get_vfunc<material_t* (*)(void*, const char*, void*)>(this, 83)(this, name, kv);
    }
    auto find_material(const char* name, const char* group_name)
    {
        return memory::get_vfunc<material_t* (*)(void*, const char*, const char*, bool, const char*)>(this, 84)(this, name, group_name, true, nullptr);
    }
    auto first_material()
    {
        return memory::get_vfunc<material_handle_t (*)(void*)>(this, 86)(this);
    }
    auto next_material()
    {
        return memory::get_vfunc<material_handle_t (*)(void*)>(this, 87)(this);
    }
    auto invalid_material()
    {
        return memory::get_vfunc<material_handle_t (*)(void*)>(this, 88)(this);
    }
    auto get_material(material_handle_t h)
    {
        return memory::get_vfunc<material_t* (*)(void*, material_handle_t)>(this, 89)(this, h);
    }
    auto find_texture(const char* name, const char* group, bool complain = true, const char* prefix = nullptr)
    {
        return memory::get_vfunc<texture_t* (*)(void*, const char*, const char*, bool, const char*)>(this, 91)(this, name, group, complain, prefix);
    }
    auto get_render_context()
    {
        return memory::get_vfunc<material_render_context_t* (*)(void*)>(this, 115)(this);
    }
    auto create_named_render_target_texture_ex(const char* name, int w, int h, int mode, int format, int depth, unsigned int texture_flags, unsigned int render_target_flags)
    {
        return memory::get_vfunc<texture_t* (*)(void*, const char*, int, int, int, int, int, unsigned int, unsigned int)>(
          this,
          99)(this, name, w, h, mode, format, depth, texture_flags, render_target_flags);
    }
};