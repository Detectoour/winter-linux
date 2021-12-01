#include "../winter.h"
#include "interfaces.h"

bool interfaces_t::setup()
{
    convar = get<cvar_t>("./bin/linux64/materialsystem_client.so", "VEngineCvar");
    if (!convar)
        return false;

    surface = get<surface_t>("./bin/linux64/vguimatsurface_client.so", "VGUI_Surface");
    if (!surface)
        return false;

    base_client = get<base_client_t>("./csgo/bin/linux64/client_client.so", "VClient");
    if (!base_client)
        return false;

    engine = get<engine_client_t>("./bin/linux64/engine_client.so", "VEngineClient");
    if (!engine)
        return false;

    entity_list = get<entity_list_t>("./csgo/bin/linux64/client_client.so", "VClientEntityList");
    if (!entity_list)
        return false;

    studio_render = get<studio_render_t>("./bin/linux64/studiorender_client.so", "VStudioRender");
    if (!studio_render)
        return false;

    model_render = get<model_render_t>("./bin/linux64/engine_client.so", "VEngineModel");
    if (!model_render)
        return false;

    model_info = get<model_info_t>("./bin/linux64/engine_client.so", "VModelInfoClient");
    if (!model_info)
        return false;

    material_system = get<material_system_t>("./bin/linux64/materialsystem_client.so", "VMaterialSystem");
    if (!material_system)
        return false;

    render_view = get<render_view_t>("./bin/linux64/engine_client.so", "VEngineRenderView");
    if (!render_view)
        return false;

    static_prop_mgr = get<static_prop_manager_t>("./bin/linux64/engine_client.so", "StaticPropMgrClient");
    if (!static_prop_mgr)
        return false;

    debug_overlay = get<debug_overlay_t>("./bin/linux64/engine_client.so", "VDebugOverlay");
    if (!debug_overlay)
        return false;

    localize = get<localize_t>("./bin/linux64/localize_client.so", "Localize_");
    if (!localize)
        return false;

#define CHECK_SANITY(addr)                                                                                                                                                                     \
    if (!memory::is_valid_address(static_cast<address_t>(addr)))                                                                                                                               \
    {                                                                                                                                                                                          \
        logger::log(fmt::format("The address of {} is invalid.", #addr));                                                                                                                      \
        return false;                                                                                                                                                                          \
    }                                                                                                                                                                                          \
    else                                                                                                                                                                                       \
    {                                                                                                                                                                                          \
        logger::log(fmt::format("{} ==> {:#09X}", #addr, (static_cast<address_t>(addr)).m_ptr));                                                                                               \
    }

    global = memory::get_vfunc(base_client, 11).add(13).relative(0x3).self_get().cast<global_vars*>();
    CHECK_SANITY(global)

    auto client_mode = memory::get_vfunc(base_client, 10).add(11).relative().cast<void* (*)()>()();
    CHECK_SANITY(client_mode)

    hud_chat = game::addresses.hud_chat.cast<hud_chat_t* (*)()>()();
    CHECK_SANITY(hud_chat)

    // Search string "<UNPAINTED>" and the function without any parameter is GetItemSchema()
    item_schema = game::addresses.item_schema.cast<item_schema_t* (*)()>()();
    CHECK_SANITY(item_schema)

    game::addresses.hook_frame_stage_notify = memory::get_vfunc(base_client, 37);
    CHECK_SANITY(game::addresses.hook_frame_stage_notify)

    game::addresses.hook_createmove = memory::get_vfunc(client_mode, 25);
    CHECK_SANITY(game::addresses.hook_createmove)

    game::addresses.hook_post_screen_effects = memory::get_vfunc(client_mode, 45);
    CHECK_SANITY(game::addresses.hook_post_screen_effects)

    game::addresses.hook_level_shutdown = memory::get_vfunc(base_client, 7);
    CHECK_SANITY(game::addresses.hook_level_shutdown)

    return true;
}