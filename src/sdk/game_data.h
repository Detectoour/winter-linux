#pragma once

#include <unordered_map>

struct weapon_data_t
{
    explicit weapon_data_t(item_schema_t::econ_item_definition_t* item);
    void print();

    std::string weapon_name;
    std::string localize_name;
    float spread {};
    float range_modifier {};
    float penetration {};
    int damage {};
    float range {};
    float armor_ratio {};
};

struct skin_data_t
{
    int id {};
    std::string name {};
    int rarity;
};

struct game_data_t
{
    game_data_t() = default;
    void initialize();

    std::unordered_map<int, weapon_data_t> weapon_data;
    std::vector<skin_data_t> skin_kits {};
    std::vector<skin_data_t> glove_kits {};
};

namespace game
{
    inline game_data_t game_data;
}