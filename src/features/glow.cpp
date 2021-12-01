#include "../winter.h"
#include "glow.h"

texture_t* full_frame;
texture_t* small_fb0;
texture_t* small_fb1;

material_t* glow_mat;
material_t* halo_mat;
material_t* down_mat;
material_t* blur_x;
material_t* blur_y;

void features::glow::initialize()
{
    full_frame = game::interfaces.material_system->find_texture("_rt_FullFrameFB", "RenderTargets");
    small_fb0  = game::interfaces.material_system->find_texture("_rt_SmallFB0", "RenderTargets");
    small_fb1  = game::interfaces.material_system->find_texture("_rt_SmallFB1", "RenderTargets");

    glow_mat = game::interfaces.material_system->find_material("dev/glow_color", "Other textures");
    halo_mat = game::interfaces.material_system->find_material("dev/halo_add_to_screen", "Other textures");
    down_mat = game::interfaces.material_system->find_material("dev/glow_downsample", "Other textures");
    blur_x   = game::interfaces.material_system->find_material("dev/glow_blur_x", "Other textures");
    blur_y   = game::interfaces.material_system->find_material("dev/glow_blur_y", "Other textures");
}

void features::glow::render()
{
    if (!game::interfaces.engine->in_game())
        return;

    const auto ctx = game::interfaces.material_system->get_render_context();
    if (!ctx)
        return;

    auto local = game::interfaces.entity_list->get<cs_player*>(game::interfaces.engine->get_local_player());
    if (!local)
        return;

    if (!ents.empty())
        ents.clear();

    game::interfaces.studio_render->forced_material_override(glow_mat);
    shader_stencil_state_t ss_disabled = {};
    ss_disabled.enable                 = false;

    float saved_clr[3];
    game::interfaces.render_view->get_color(saved_clr);
    const auto saved_alpha = game::interfaces.render_view->get_alpha();

    game::interfaces.render_view->set_alpha(0.f);
    ctx->override_depth_enable(true, false);

    shader_stencil_state_t ss_occluded_unoccluded = {};
    ss_occluded_unoccluded.enable                 = true;
    ss_occluded_unoccluded.reference_value        = 1;
    ss_occluded_unoccluded.compare_func           = shader_stencil_func_t::SHADER_STENCILFUNC_ALWAYS;
    ss_occluded_unoccluded.pass_op                = shader_stencil_op_t::SHADER_STENCILOP_SET_TO_REFERENCE;
    ss_occluded_unoccluded.fail_op                = shader_stencil_op_t::SHADER_STENCILOP_KEEP;
    ss_occluded_unoccluded.z_fail_op              = shader_stencil_op_t::SHADER_STENCILOP_SET_TO_REFERENCE;

    auto draw_model = [](cs_player* ent)
    {
        if (!ent->should_draw())
            return;
        ent->draw_model(STUDIO_RENDER | STUDIO_SKIP_FLEXES | STUDIO_DONOTMODIFYSTENCILSTATE | STUDIO_NOLIGHTING_OR_CUBEMAP | STUDIO_SKIP_DECALS, 255);
        /*        auto attachment = ent->first_move_child();
        while ( attachment )
        {
            if ( attachment->should_draw() )
                attachment->draw_model( STUDIO_RENDER | STUDIO_SKIP_FLEXES | STUDIO_DONOTMODIFYSTENCILSTATE |
        STUDIO_NOLIGHTING_OR_CUBEMAP | STUDIO_SKIP_DECALS, 255 ); attachment = attachment->next_move_peer();
        }*/
    };

    for (auto i = 1; i <= game::interfaces.global->max_clients; ++i)
    {
        auto ent = game::interfaces.entity_list->get<cs_player*>(i);
        if (!ent || ent == local || ent->networkable()->dormant() || !ent->alive())
            continue;
        ctx->set_stencil_state(ss_occluded_unoccluded);
        draw_model(ent);
        ents.push_back(ent);
    }

    game::interfaces.studio_render->forced_material_override(nullptr);
    game::interfaces.render_view->set_alpha(0.f);

    ctx->override_depth_enable(false, false);
    game::interfaces.render_view->set_alpha(saved_alpha);
    ctx->set_stencil_state(ss_disabled);

    if (ents.empty())
        return;

    int x = static_cast<int>(cheat::screen_size.x);
    int y = static_cast<int>(cheat::screen_size.y);
    // Render the glow colors to _rt_FullFrameFB
    {
        ctx->begin_pix_event(0xFFF5940F, "0xFFF5940F");
        {
            ctx->push_render_target_and_viewport();
            {
                float saved_clr_[3];
                game::interfaces.render_view->get_color(saved_clr_);
                const auto saved_alpha_ = game::interfaces.render_view->get_alpha();
                ctx->set_render_target(full_frame);
                ctx->view_port(0, 0, x, y);

                ctx->clear_3ub(0, 0, 0);
                ctx->clear_buffers(true, false, false);
                game::interfaces.studio_render->forced_material_override(glow_mat);
                float red[3] = { 1.f, 0.f, 0.f };
                for (auto i : ents)
                {
                    game::interfaces.render_view->set_color(color_t(red));
                    game::interfaces.render_view->set_alpha(1.f);
                    draw_model(i);
                }

                game::interfaces.studio_render->forced_material_override(nullptr);
                game::interfaces.render_view->set_color(color_t(saved_clr_));
                game::interfaces.render_view->set_alpha(saved_alpha_);
                ctx->set_stencil_state(ss_disabled);
            }
            ctx->pop_render_target_and_viewport();
        }
    }

    // Down sample
    {
        int vp_x, vp_y, vp_w, vp_h;
        ctx->get_view_port(vp_x, vp_y, vp_w, vp_h);
        if (small_fb0->get_actual_width() != x / 4 || small_fb0->get_actual_height() != y / 4)
        {
            ctx->set_render_target(small_fb0);
            ctx->view_port(0, 0, x, y);
            ctx->clear_3ub(0, 0, 0);
            ctx->clear_buffers(true, false, false);
        }

        ctx->set_render_target(small_fb0);
        ctx->view_port(0, 0, x / 4, y / 4);
        auto bloom_exp = down_mat->find_var("$bloomexp", nullptr);
        if (bloom_exp)
            bloom_exp->set_float(2.5f);
        auto bloom_saturation = down_mat->find_var("$bloomsaturation", nullptr);
        if (bloom_saturation)
            bloom_saturation->set_float(1.f);
        ctx->draw_screen_space_rectangle(down_mat, 0, 0, x / 4, y / 4, 0, 0, x - 4, y - 4, full_frame->get_actual_width(), full_frame->get_actual_height());
    }

    auto dim = halo_mat->find_var("$C0_X", nullptr);
    dim->set_float(1.f);

    shader_stencil_state_t ss;
    ss.enable     = true;
    ss.write_mask = 0x0; // We're not changing stencil
    // stencilState.m_nTestMask = 0x3;
    ss.reference_value = 0x0;
    ss.compare_func    = shader_stencil_func_t::SHADER_STENCILFUNC_EQUAL;
    ss.pass_op         = shader_stencil_op_t::SHADER_STENCILOP_KEEP;
    ss.fail_op         = shader_stencil_op_t::SHADER_STENCILOP_KEEP;
    ss.z_fail_op       = shader_stencil_op_t::SHADER_STENCILOP_KEEP;
    ctx->set_stencil_state(ss);

    int vp_x, vp_y, vp_w, vp_h;
    ctx->get_view_port(vp_x, vp_y, vp_w, vp_h);
    ctx->draw_screen_space_rectangle(halo_mat, 0, 0, vp_w, vp_h, 0, -0.5f, x / 4 - 1, y / 4 - 1, small_fb1->get_actual_width(), small_fb1->get_actual_height());
    ctx->set_stencil_state(ss_disabled);
}
