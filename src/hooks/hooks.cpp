#include "../winter.h"
#include "hooks.h"
#include "../features/chams.h"
#include "../features/glow.h"
#include "../features/world.h"
#include "../features/movement.h"
#include "../config.h"
#include "../utils/math.h"

using namespace hook_funcs;

void paint::hook(void* ptr, int mode)
{
    detour.call_original<decltype(&hook)>(ptr, mode);
    int w, h;
    game::interfaces.engine->get_screen_size(w, h);
    cheat::screen_size = vec2_t { w, h };

    if (game::interfaces.engine->in_game())
        cheat::matrix = game::interfaces.engine->world_to_screen_matrix();

    if (mode & 1)
    {
        static auto sd = game::addresses.start_drawing.cast<void (*)(void*)>();
        static auto fd = game::addresses.finish_drawing.cast<void (*)(void*)>();
        sd(game::interfaces.surface);
        {
            // Do surface stuff here
        }
        fd(game::interfaces.surface);
    }
}

bool clientmode_createmove::hook(void* ptr, float frame_time, user_cmd* cmd)
{
    detour.call_original<decltype(&hook)>(ptr, frame_time, cmd);

    if (cmd && cmd->command_number)
    {
        auto local_player = game::interfaces.entity_list->get<cs_player*>(game::interfaces.engine->get_local_player());
        if (!cheat::local)
            cheat::local = local_player;
        features::movement::autobhop(local_player, cmd);
        features::movement::airstrafe(local_player, cmd);

        // TODO: Engine prediction (maybe rebuild runcommand?)
        {
        }

        cmd->view_angles.normalize();
        cmd->forward_move = std::clamp(cmd->forward_move, -450.f, 450.f);
        cmd->side_move    = std::clamp(cmd->side_move, -450.f, 450.f);
        cmd->up_move      = std::clamp(cmd->up_move, -320.f, 320.f);
    }

    // Return false because we don't want to set our angles (especially when making anti-aim)
    // If you want to make your aimbot "visible", please do game::interfaces.engine->set_view_angles(angle);
    return false;
}

void level_init_post::hook(void* ptr)
{
    detour.call_original<decltype(&hook)>(ptr);

    if (!cheat::local)
    {
        auto local_player = game::interfaces.entity_list->get<cs_player*>(game::interfaces.engine->get_local_player());
        cheat::local      = local_player;
    }

    if (!game::netvar.m_dumped)
    {
        game::netvar.dump_datamap();
    }

    features::world::prop_color();
}

void draw_model::hook(void* ptr, void* result, const draw_model_info_t* info, matrix3x4_t* bone_to_world, float* weight, float* delayed_weight, const vec3_t* origin, int flags)
{
    if (game::interfaces.studio_render->is_overried())
    {
        detour.call_original<decltype(&hook)>(ptr, result, info, bone_to_world, weight, delayed_weight, origin, flags);
        return;
    }

    auto ret = features::chams::draw_model_pre(ptr, result, *info, bone_to_world, weight, delayed_weight, origin, flags);
    detour.call_original<decltype(&hook)>(ptr, result, info, bone_to_world, weight, delayed_weight, origin, flags);
    if (ret)
    {
        ret = features::chams::draw_model_post(ptr, result, *info, bone_to_world, weight, delayed_weight, origin, flags);
        if (ret)
        {
            detour.call_original<decltype(&hook)>(ptr, result, info, bone_to_world, weight, delayed_weight, origin, flags);
            game::interfaces.studio_render->forced_material_override(nullptr);
        }
    }
}

void post_screen_effects::hook(void* ptr, void* view_setup)
{
    // Doesn't work :pensive:
    // features::glow::render();

    detour.call_original<decltype(&hook)>(ptr, view_setup);
}

void color_modulate::hook(void* ptr, float* r, float* g, float* b)
{
    detour.call_original<decltype(&hook)>(ptr, r, g, b);

    if (!cheat::config.get<bool>("esp.nightmode"))
        return;

    const auto material = reinterpret_cast<material_t*>(ptr);
    const auto group    = material->get_texture_group_name();
    const auto name     = material->get_name();

    if (strstr(group, "Other") || strstr(group, "StaticProp") || strstr(name, "weapon") || strstr(name, "glow") || strstr(name, "chams") || strstr(name, "player"))
        return;

    *r *= 0.2f;
    *g *= 0.2f;
    *b *= 0.2f;
}

void level_shutdown::hook(void* ptr)
{
    cheat::local = nullptr;
    features::world::clear();

    detour.call_original<decltype(&hook)>(ptr);
}

void frame_stage_notify::hook(void* ptr, client_frame_stage_t stage)
{
    detour.call_original<decltype(&hook)>(ptr, stage);
}

// Credit: =＾● ⋏ ●＾= (nicole)
int get_whitelist_spew_flags::hook(void* ptr)
{
    return 1;
}

float get_fov::hook(cs_player* ptr)
{
    // Could have used player->GetDefaultFov, but it only returns the convar value if I remember correctly
    static auto default_fov = game::interfaces.convar->find_var("default_fov");
    if (!ptr->active_weapon())
    {
        return default_fov ? default_fov->get_float() : 90.f;
    }
    return detour.call_original<float, cs_player*>(ptr);
}

bool hooks::hook()
{
    features::chams::initialize();
    features::glow::initialize();
    try
    {
        create(paint::detour, game::addresses.hook_paint, reinterpret_cast<void*>(paint::hook), "CEngineVGui::Paint");
        create(clientmode_createmove::detour, game::addresses.hook_createmove, reinterpret_cast<void*>(clientmode_createmove::hook), "IClientModeShared::CreateMove");
        create(level_init_post::detour, game::addresses.hook_level_init_post, reinterpret_cast<void*>(level_init_post::hook), "CHLClient::LevelInitPostEntity");
        create(draw_model::detour, game::addresses.hook_draw_model, reinterpret_cast<void*>(draw_model::hook), "CStudioRenderContext::DrawModel");
        create(post_screen_effects::detour, game::addresses.hook_post_screen_effects, reinterpret_cast<void*>(post_screen_effects::hook), "IClientModeShared::PostScreenEffects");
        create(color_modulate::detour, game::addresses.hook_material_get_color_modulate, reinterpret_cast<void*>(color_modulate::hook), "CMaterial::GetColorModulate");
        create(level_shutdown::detour, game::addresses.hook_level_shutdown, reinterpret_cast<void*>(level_shutdown::hook), "CHLClient::LevelShutdown");
        create(frame_stage_notify::detour, game::addresses.hook_frame_stage_notify, reinterpret_cast<void*>(frame_stage_notify::hook), "CHLClient::FrameStageNotify");
        create(get_fov::detour, game::addresses.hook_get_fov, reinterpret_cast<void*>(get_fov::hook), "C_BasePlayer::GetFov");
        create(get_whitelist_spew_flags::detour,
               game::addresses.hook_get_whitelist_spew_flags,
               reinterpret_cast<void*>(get_whitelist_spew_flags::hook),
               "CBaseFileSystem::GetWhitelistSpewFlags");
    }
    catch (std::runtime_error& e)
    {
        logger::log(e.what(), log_level::error);
        return false;
    }
    return true;
}

void hooks::unhook()
{
    destroy(paint::detour, "CEngineVGui::Paint");
    destroy(clientmode_createmove::detour, "IClientModeShared::CreateMove");
    destroy(level_init_post::detour, "CHLClient::LevelInitPostEntity");
    destroy(draw_model::detour, "CStudioRenderContext::DrawModel");
    destroy(post_screen_effects::detour, "IClientModeShared::PostScreenEffects");
    destroy(color_modulate::detour, "CMaterial::GetColorModulate");
    destroy(level_shutdown::detour, "CHLClient::LevelShutdown");
    destroy(frame_stage_notify::detour, "CHLClient::FrameStageNotify");
    destroy(get_fov::detour, "C_BasePlayer::GetFov");
    destroy(get_whitelist_spew_flags::detour, "CBaseFileSystem::GetWhitelistSpewFlags");

    features::world::prop_color(true);
}

void hooks::create(c_detour& detour, address_t address, void* func, const std::string& name)
{
    if (!detour.create(address, func) || !detour.hook())
    {
        throw std::runtime_error(fmt::format("Failed to hook {}.", name));
    }
    logger::log(fmt::format("Hooked {} ==> {:#09x}", name, address.m_ptr));
}

void hooks::destroy(c_detour& detour, const std::string& name)
{
    if (!detour.unhook())
        return;
    logger::log(fmt::format("Unhooked {}", name));
}
