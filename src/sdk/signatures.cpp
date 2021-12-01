#include "../winter.h"
#include "signatures.h"

bool signatures_t::setup_patterns()
{
// https://stackoverflow.com/a/49835440
#define INSERT(name, pattern, ...) pattern_manager[HASH(#name)] = pattern_info_t(STB((pattern)), ##__VA_ARGS__);

    // How to get the aligned pattern and offset:
    // offset = function_address % sizeof(pattern_bytes::simd_value_t)
    // pattern_to_sig_address = function_address - offset
    // Let's say my cpu only supports AVX2 instruction, so the size of pattern_bytes::simd_value_t is 32 (uint64_t is 8 and AVX512 is 64)
    // and the function_address is 0xCC7127 in client_client.so (GetPaintKitDefinitionByIndex)
    // so the offset is 0xCC7127 % 32 = 0x7
    // then the address we need to sig is 0xCC7127 - 0x7 = 0xCC7120
    // so we need to sig at 0xCC7127, which will give us the pattern "48 89 C7 F3 0F 2C F0"
    // One thing to notice is that sometimes the address we need to sig is cannot be generated or not what we need:
    // .text:0xCF343C 0F 85 E6 00 00 00                                               jnz     loc_CF3528
    // .text:0xCF3442                                                 loc_CF3442:
    // .text:0xCF3442 45 8B 8C 24 90 01 00 00                                         mov     r9d, [r12+190h]
    // .text:0xCF344A 31 DB                                                           xor     ebx, ebx
    // .text:0xCF344C 45 85 C9                                                        test    r9d, r9d
    // .text:0xCF344F 7E 43                                                           jle     short loc_CF3494
    // .text:0xCF3451                                                 loc_CF3451:
    // .text:0xCF3451 49 8B 84 24 80 01 00 00                                         mov     rax, [r12+180h]
    // Let's say the actual address we need is 0xCF3451, so after the calculation we know that the address we need to sig is 0xCF3440
    // But you will find that your sigmaker only gives you the result for 0xCF343C(0F 85 ? ? ? ? 45 8B 8C 24 ? ? ? ? 31 DB) not 0xCF3440
    // So here is what you should do:
    // bytes_to_skip = pattern_to_sig_address - result_address
    // which will be: bytes_to_skip = 0xCF3440 - 0xCF343C = 4
    // so the pattern we should use should be: "00 00 45 8B 8C 24 80 01 00 00 31 DB" or "? ? 45 8B 8C 24 ? ? ? ? 31 DB"
    // Because the first byte of some aligned addresses can actually be unknown

#ifdef __AVX512F__
    INSERT(hook_paint, "89 DE 88 45 C0 48 8B 07", 0x30)
    INSERT(hook_level_init_post, "48 8B 52 38 FF E2 66 2E 0F 1F 84 00 00 00 00 00", 0x10)
    INSERT(hook_draw_model, "55 48 89 E5 41 57 49 89 D7 41 56 4D 89 C6")
    INSERT(hook_material_get_color_modulate, "6F 80 7B 2A 04 0F 57 C0", 0x20)
    INSERT(hook_get_whitelist_spew_flags, "55 B8 ? ? ? ? 48 89 E5 5D C3 90 90 90 90 90 55 48 8D 05 ? ? ? ? 48 89 E5 48 89 07 5D C3 55 48 8D 05", 0x20)
    INSERT(hook_get_fov, "48 D9 05 48 83 EC 08", 0x7, true, true)

    INSERT(paintkit_definition, "05 CB C0 B6 00 F3 0F 11 85", 0x27, true, true)
    INSERT(item_schema, "48 83 EC 18 48 89 75 C8 8B 47 60", 0x2C, true, true)
    INSERT(hud_chat, "55 48 8B 05 ? ? ? ? 48 89 E5 5D C3 90 66 90 55 48 8B 87")
    INSERT(offset_item_definition, "8B 97 ? ? ? ? 85 D2 7E DD", 0x18)
    INSERT(offset_schema_attribute, "? ? 45 8B 8C 24 ? ? ? ? 31 DB", 0x11)
    INSERT(offset_item_attribute, "48 83 EC 18 48 89 75 C8 8B 47 60", 0x1B, true)
    INSERT(start_drawing, "48 89 DF E8 ? ? ? ? 48 8D BD ? ? ? ? 48 89 C3", 0x20)
    INSERT(finish_drawing, "50 08 EB E4 66 66 66 2E 0F 1F 84 00 00 00 00 00", 0x10)
    INSERT(init_kv, "85 90 FC FF FF 4D", 0x3E, true, true)
    INSERT(load_from_buffer, "48 81 EC ? ? ? ? 48 85 D2 48 89 BD", 0x10, true)
    INSERT(draw_model, "A3 01 8B 83 0C 10 00 00 85", 0x20, true, true)
    INSERT(should_draw, "55 48 89 E5 53 48 89 FB 48 83 EC 08 80 BF ? ? ? ? ? 74 23 E8 ? ? ? ? 0F B6 93")
    INSERT(set_clantag, "49 89 F4 53 48 83 EC 08 E8 ? ? ? ? 48 8D 35 ? ? ? ? 48 89 C7 48 89 C3 E8 ? ? ? ? 48 8D 35 ? ? ? ? 4C 89 EA", 0x10)
    INSERT(weapon_update_accuracy_penalty, "55 48 89 E5 53 48 89 FB 48 83 EC 08 E8 ? ? ? ? 48 8B 03 48 89 DF FF 90 ? ? ? ? 0F B6 80 ? ? ? ? 48 83 C4 08 5B 5D C3 90 90 90 90 90 48 8D 05", 0x30)
    INSERT(weapon_info, "3A 00 00 0F 84 60 FF", 0x30, true)
    INSERT(player_has_c4, "00 48 8B BD 78 FF FF FF E8 ? ? ? ? 84 C0", 0x8, true, true)
    INSERT(player_active_weapon, "55 48 85 FF 48 89 E5 41 54 53 48 89 FB 74 0D 48 8B 07 FF 90 ? ? ? ? 84 C0 75 14", 0x26, true, true)
    INSERT(player_alive, "48 83 EF 08 EB EA 90 90 90 90 90 90 90 90 90 90 55 48 89 E5 53 48 89 FB 48 83 EC 08 80 3D", 0x20)
    INSERT(player_ground_entity, "55 48 89 E5 41 56 41 55 49 89 F5 41 54 53 48 89 FB 48 83 EC 10 48 85 F6", 0x26, true, true)
#elif defined(__AVX2__)
    INSERT(hook_paint, "? ? ? 48 83 C4 28 5B 41 5C 41 5D 5D C3 66 90", 0x10)
    INSERT(hook_level_init_post, "48 8B 52 38 FF E2 66 2E 0F 1F 84 00 00 00 00 00", 0x10)
    INSERT(hook_draw_model, "55 48 89 E5 41 57 49 89 D7 41 56 4D 89 C6")
    INSERT(hook_material_get_color_modulate, "55 48 89 E5 41 56 49 89 F6 41 55 49 89 D5")
    INSERT(hook_get_whitelist_spew_flags, "55 8B 87 ? ? ? ? 48 89 E5 5D C3 90")
    INSERT(hook_get_fov, "48 D9 05 48 83 EC 08", 0x7, true, true)

    INSERT(paintkit_definition, "48 89 C7 F3 0F 2C F0", 0x7, true, true)
    INSERT(item_schema, "80 7C 24 10 00 75 41", 0xC, true, true)
    INSERT(hud_chat, "55 48 8B 05 ? ? ? ? 48 89 E5 5D C3 90 66 90 55 48 8B 87")
    INSERT(offset_schema_attribute, "? ? 45 8B 8C 24 ? ? ? ? 31 DB", 0x11)
    INSERT(offset_item_definition, "8B 97 ? ? ? ? 85 D2 7E DD", 0x18)
    INSERT(offset_item_attribute, "48 83 EC 18 48 89 75 C8 8B 47 60", 0x1B)
    INSERT(start_drawing, "55 48 89 E5 53 48 89 FB 48 83 EC 28 80 3D")
    INSERT(finish_drawing, "50 08 EB E4 66 66 66 2E 0F 1F 84 00 00 00 00 00", 0x10)
    INSERT(init_kv, "FF 00 4C 03 20 C6 85", 0x1E, true, true)
    INSERT(load_from_buffer, "55 48 89 E5 41 57 41 56 41 55 41 54 49 89 D4 53 48 81 EC ? ? ? ? 48 85 D2")
    INSERT(draw_model, "55 48 89 E5 41 57 41 89 F7 41 56 41 55 41 54 49 89 FC 53 48 83 EC 38")
    INSERT(should_draw, "55 48 89 E5 53 48 89 FB 48 83 EC 08 80 BF ? ? ? ? ? 74 23 E8 ? ? ? ? 0F B6 93")
    INSERT(set_clantag, "49 89 F4 53 48 83 EC 08 E8 ? ? ? ? 48 8D 35 ? ? ? ? 48 89 C7 48 89 C3 E8 ? ? ? ? 48 8D 35 ? ? ? ? 4C 89 EA", 0x10)
    INSERT(weapon_update_accuracy_penalty,
           "55 48 89 E5 53 48 89 FB 48 83 EC 08 E8 ? ? ? ? 48 8B 03 48 89 DF FF 90 ? ? ? ? 0F B6 80 ? ? ? ? 48 83 C4 08 5B 5D C3 90 90 90 90 90 55 48 89 E5 53 48 89 FB 48 83 EC 08 E8 ? ? ? ? "
           "48 8B 03 48 89 DF FF 90 ? ? ? ? 0F B6 80 ? ? ? ? 48 83 C4 08 5B 5D C3 90 90 90 90 90 48 8D 05")
    INSERT(weapon_info, "F3 0F 11 83 ? ? ? ? EB C2 66 0F 1F 44 00 00 48 8B 87", 0x10, true)
    INSERT(player_has_c4, "00 48 8B BD 78 FF FF FF E8 ? ? ? ? 84 C0", 0x8, true, true)
    INSERT(player_active_weapon, "4C 89 E7 41 5C 5D E9 ? ? ? ? 0F 1F 44 00 00 80 BB", 0x6, true, true)
    INSERT(player_alive, "80 BF ? ? ? ? ? 55 48 89 E5 5D 0F 94 C0")
    INSERT(player_ground_entity, "7F 08 4C 8B 66 58", 0x6, true, true)
#else
    INSERT(hook_paint, "55 48 89 E5 41 57 41 56 41 55 41 54 49 89 FC 53 48 83 EC 68 89 75 9C")
    INSERT(hook_level_init_post, "55 48 89 E5 41 54 53 E8 ? ? ? ? BF")
    INSERT(hook_draw_model, "55 48 89 E5 41 57 49 89 D7 41 56 4D 89 C6")
    INSERT(hook_material_get_color_modulate, "55 48 89 E5 41 56 49 89 F6 41 55 49 89 D5")
    INSERT(hook_get_whitelist_spew_flags, "55 8B 87 ? ? ? ? 48 89 E5 5D C3 90")
    INSERT(hook_get_fov, "48 D9 05 48 83 EC 08", 0x7, true, true)

    INSERT(paintkit_definition, "48 89 C7 F3 0F 2C F0", 0x7, true, true)
    INSERT(item_schema, "0F B7 3C 24 E8 ? ? ? ? 41 0F B7 CF", 0x4, true, true)
    INSERT(hud_chat, "55 48 8B 05 ? ? ? ? 48 89 E5 5D C3 90 66 90 55 48 8B 87")
    INSERT(offset_item_definition, "49 03 84 24 ? ? ? ? 83 78 10 FF")
    INSERT(offset_schema_attribute, "43 49 8B 84 24 80 01 00 00", 0x1, true)
    INSERT(offset_item_attribute, "4D 89 EC 4D 03 66 50", 0x3, true)
    INSERT(start_drawing, "55 48 89 E5 53 48 89 FB 48 83 EC 28 80 3D")
    INSERT(finish_drawing, "55 31 FF 48 89 E5 53")
    INSERT(init_kv, "48 89 C3 4C 89 FE E8 ? ? ? ? 49 8B 06", 0x6, true, true)
    INSERT(load_from_buffer, "55 48 89 E5 41 57 41 56 41 55 41 54 49 89 D4 53 48 81 EC ? ? ? ? 48 85 D2")
    INSERT(draw_model, "55 48 89 E5 41 57 41 89 F7 41 56 41 55 41 54 49 89 FC 53 48 83 EC 38")
    INSERT(should_draw, "55 48 89 E5 53 48 89 FB 48 83 EC 08 80 BF ? ? ? ? ? 74 23 E8 ? ? ? ? 0F B6 93")
    INSERT(set_clantag, "55 48 89 E5 41 55 49 89 FD 41 54 BF ? ? ? ? 49 89 F4 53 48 83 EC 08 E8 ? ? ? ? 48 8D 35 ? ? ? ? 48 89 C7 48 89 C3 E8 ? ? ? ? 48 8D 35 ? ? ? ? 4C 89 EA")
    INSERT(weapon_update_accuracy_penalty,
           "55 48 89 E5 53 48 89 FB 48 83 EC 08 E8 ? ? ? ? 48 8B 03 48 89 DF FF 90 ? ? ? ? 0F B6 80 ? ? ? ? 48 83 C4 08 5B 5D C3 90 90 90 90 90 55 48 89 E5 53 48 89 FB 48 83 EC 08 E8 ? ? ? ? "
           "48 8B 03 48 89 DF FF 90 ? ? ? ? 0F B6 80 ? ? ? ? 48 83 C4 08 5B 5D C3 90 90 90 90 90 48 8D 05")
    INSERT(weapon_info, "48 8B 87 ? ? ? ? 55 48 89 E5 48 85 C0 74 08 5D C3 66 0F 1F 44 00 00 5D E9 ? ? ? ? 90", 0, true)
    INSERT(player_has_c4, "E8 ? ? ? ? 84 C0 74 5F 48 8B 85 ? ? ? ? 4C 89 F1", 0, true, true)
    INSERT(player_active_weapon, "4C 89 E7 41 5C 5D E9 ? ? ? ? 0F 1F 44 00 00 80 BB", 0x6, true, true)
    INSERT(player_alive, "80 BF ? ? ? ? ? 55 48 89 E5 5D 0F 94 C0")
    INSERT(player_ground_entity, "7F 08 4C 8B 66 58", 0x6, true, true)
#endif

    return true;
}

bool signatures_t::setup()
{
#define GET_ADDRESS(_module, addr)                                                                                                                                                             \
    addr = modules::_module.find_pattern(pattern_manager[HASH(#addr)]);                                                                                                                        \
    if (!memory::is_valid_address(addr))                                                                                                                                                       \
    {                                                                                                                                                                                          \
        logger::log(fmt::format("The address of {} is invalid.", #addr));                                                                                                                      \
        return false;                                                                                                                                                                          \
    }                                                                                                                                                                                          \
    else                                                                                                                                                                                       \
    {                                                                                                                                                                                          \
        logger::log(fmt::format("{} ==> {:#09X}", #addr, (addr).m_ptr));                                                                                                                       \
    }

    // Hooks
    {
        /*
          Abandoned because it sets your viewangles, which you don't really want to happen when making anti-aim
          hook_createmove = modules::client.find_pattern("55 0F 28 C8 48 89 E5 41 54 49 89 F4");
          GET_ADDRESS(hook_createmove);
        */
        GET_ADDRESS(engine, hook_paint);
        GET_ADDRESS(client, hook_level_init_post);
        GET_ADDRESS(studio_render, hook_draw_model);
        GET_ADDRESS(material_system, hook_material_get_color_modulate);
        GET_ADDRESS(file_system, hook_get_whitelist_spew_flags);
        GET_ADDRESS(client, hook_get_fov);
    }

    // Functions
    {
        GET_ADDRESS(client, hud_chat);
        GET_ADDRESS(client, offset_schema_attribute); // Original pattern: 49 8B 84 24 ? ? ? ? 4C 8D 3C DD ? ? ? ? 48 8B 3C D8 48 85 FF
        GET_ADDRESS(client, offset_item_definition);
        GET_ADDRESS(client, offset_item_attribute); // Original pattern: 4D 03 66 50 41 80 7C 24
        GET_ADDRESS(client, paintkit_definition);   // Original pattern: E8 ? ? ? ? 48 83 C0 50
        GET_ADDRESS(client, item_schema);           // Original pattern: E8 ? ? ? ? 8B 58 20
        GET_ADDRESS(matsurface, start_drawing);
        GET_ADDRESS(matsurface, finish_drawing);
        GET_ADDRESS(client, init_kv);
        GET_ADDRESS(client, load_from_buffer);
        GET_ADDRESS(client, draw_model);
        GET_ADDRESS(client, should_draw);
        GET_ADDRESS(engine, set_clantag);
        GET_ADDRESS(client, weapon_update_accuracy_penalty);
        GET_ADDRESS(client, weapon_info);   // Original pattern: 48 8B 87 90 3C 00 00
        GET_ADDRESS(client, player_has_c4); // Original pattern: E8 ? ? ? ? 45 0F B6 BD
        GET_ADDRESS(client, player_active_weapon);
        GET_ADDRESS(client, player_alive);
        GET_ADDRESS(client, player_ground_entity);
    }
    return true;
}
