#pragma once

#include <tsl/hopscotch_map.h>

struct signatures_t
{
    bool setup_patterns();
    bool setup();

    tsl::hopscotch_map<std::size_t, pattern_info_t> pattern_manager;

    address_t hook_paint {};
    address_t hook_createmove {};
    address_t hook_level_init_post {};
    address_t hook_draw_model {};
    address_t hook_post_screen_effects;
    address_t hook_material_get_color_modulate {};
    address_t hook_level_shutdown;
    address_t hook_frame_stage_notify;
    address_t hook_get_whitelist_spew_flags {};
    address_t hook_get_fov {};

    address_t item_schema {};
    address_t hud_chat {};

    address_t paintkit_definition {};
    address_t offset_item_definition {};
    address_t offset_schema_attribute {};
    address_t offset_item_attribute {};

    address_t start_drawing {};
    address_t finish_drawing {};
    address_t init_kv {};
    address_t load_from_buffer {};
    address_t set_clantag {};

    address_t draw_model {};
    address_t should_draw {};

    address_t weapon_update_accuracy_penalty {};
    address_t weapon_info {};

    address_t player_active_weapon {};
    address_t player_has_c4 {};
    address_t player_alive {};
    address_t player_ground_entity {};
};

namespace game
{
    inline signatures_t addresses;
}

#define OFFSET(func_type, func, offset)                                                                                                                                                        \
    [[nodiscard]] std::add_lvalue_reference_t<func_type> func()                                                                                                                                \
    {                                                                                                                                                                                          \
        return *(std::add_pointer_t<func_type>)(reinterpret_cast<uintptr_t>(this) + (offset));                                                                                                 \
    }
