#include "../winter.h"
#include "game_data.h"
#include "../utils/string_helper.h"

weapon_data_t::weapon_data_t(item_schema_t::econ_item_definition_t* item)
{
    std::string base_name = item->get_item_base_name();
    localize_name         = game::interfaces.localize->find(base_name.data());

    weapon_name = "weapon_" + base_name.substr(strlen("#SFUI_WPNHUD_"));
    str_helper::lowercase(weapon_name);
    std::replace(weapon_name.begin(), weapon_name.end(), ' ', '_');
    std::replace(weapon_name.begin(), weapon_name.end(), '-', '_');

#define DEFINE_ID(name, attribute_name) static auto id_##name = game::interfaces.item_schema->get_attribute_id_by_name(attribute_name);

    DEFINE_ID(spread, "spread")
    DEFINE_ID(range_modifier, "range modifier")
    DEFINE_ID(penetration, "penetration")
    DEFINE_ID(damage, "damage")
    DEFINE_ID(range, "range")
    DEFINE_ID(armor_ratio, "armor ratio")

#define GET_ATTRIBUTE(name, v_type)                                                                                                                                                            \
    if (i.id == id_##name)                                                                                                                                                                     \
        (name) = i.value.v_type;                                                                                                                                                               \
    else

    for (const auto& i : item->get_item_attributes())
    {
        GET_ATTRIBUTE(spread, v_float)
        GET_ATTRIBUTE(range_modifier, v_float)
        GET_ATTRIBUTE(penetration, v_float)
        GET_ATTRIBUTE(damage, v_int)
        GET_ATTRIBUTE(armor_ratio, v_float)
        GET_ATTRIBUTE(range, v_float)
        {
            // Don't do anything here!!!!
        }
    }
}

void weapon_data_t::print()
{
    logger::log(fmt::format("LocalizedName: {} - WeaponName: {}", localize_name, weapon_name));
    logger::log(fmt::format("    Spread:{:.4f}", spread));
    logger::log(fmt::format("    RangeModifier:{:.4f}", range_modifier));
    logger::log(fmt::format("    Range:{:.4f}", range));
    logger::log(fmt::format("    ArmorRation:{:.4f}", armor_ratio));
    logger::log(fmt::format("    Penetration:{:.4f}", penetration));
    logger::log(fmt::format("    Damage:{}", damage));
}

void game_data_t::initialize()
{
    static auto non_wep_count = 0;
    for (auto i = 0; i < game::interfaces.item_schema->get_item_definition_count(); i++)
    {
        if (non_wep_count > 4)
            break;

        const auto item = game::interfaces.item_schema->get_item_definition_by_index(i);
        if (!item)
            continue;
        std::string base_name(item->get_item_base_name());
        if (base_name.find("#SFUI_WPNHUD_") == std::string::npos)
        {
            non_wep_count++;
            continue;
        }

        weapon_data.emplace(item->get_definition_index(), item);
        non_wep_count = 0;
    }

    logger::log("Finished initializing weapon_data", log_level::critical);

    const auto kit_map = game::interfaces.item_schema->get_paintkit();
    for (auto i = 0; i < kit_map->last_element; i++)
    {
        const auto paint_kit = kit_map->memory[i].value;
        if (paint_kit->id == 9001)
            continue;

        const auto name = game::interfaces.localize->find(paint_kit->item_name.buffer + 1);

        if (paint_kit->id < 10000)
            skin_kits.push_back({ paint_kit->id, name, paint_kit->rarity });
        else
            glove_kits.push_back({ paint_kit->id, name, paint_kit->rarity });
    }

    logger::log("Finished initializing skin data", log_level::critical);
}
