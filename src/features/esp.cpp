#include <ranges>
#include "../winter.h"
#include "esp.h"
#include "../utils/math.h"
#include "../imgui/imgui.h"
#include "../utils/render.h"
#include "../config.h"

struct box_t
{
    float left;
    float top;
    float right;
    float bottom;
    float width;
    float height;
};

bool world_to_screen(const vec3_t& origin, vec2_t& screen)
{
    const auto matrix = cheat::matrix;
    const auto width  = matrix[3][0] * origin.x + matrix[3][1] * origin.y + matrix[3][2] * origin.z + matrix[3][3];

    if (width < 0.001f)
        return false;

    const float inverse = 1.0f / width;

    screen.x = (matrix[0][0] * origin.x + matrix[0][1] * origin.y + matrix[0][2] * origin.z + matrix[0][3]) * inverse;
    screen.y = (matrix[1][0] * origin.x + matrix[1][1] * origin.y + matrix[1][2] * origin.z + matrix[1][3]) * inverse;

    const ImVec2 display_size = ImGui::GetIO().DisplaySize;
    screen.x                  = (display_size.x * 0.5f) + (screen.x * display_size.x) * 0.5f;
    screen.y                  = (display_size.y * 0.5f) - (screen.y * display_size.y) * 0.5f;
    return true;
}

static bool calculate_box(base_entity* ent, box_t& box)
{
    const auto mins = ent->mins();
    const auto maxs = ent->maxs();

    // clang-format off
    std::array points = { vec3_t(mins.x, mins.y, mins.z), vec3_t(mins.x, maxs.y, mins.z),
                          vec3_t(maxs.x, maxs.y, mins.z), vec3_t(maxs.x, mins.y, mins.z),
                          vec3_t(maxs.x, maxs.y, maxs.z), vec3_t(mins.x, maxs.y, maxs.z),
                          vec3_t(mins.x, mins.y, maxs.z), vec3_t(maxs.x, mins.y, maxs.z)
                        };
    // clang-format on

    auto transformed = ent->coordinate_frame();
    auto left        = std::numeric_limits<float>::max();
    auto top         = std::numeric_limits<float>::max();
    auto right       = std::numeric_limits<float>::lowest();
    auto bottom      = std::numeric_limits<float>::lowest();

    std::array<vec2_t, 8> screen;
    for (auto i = 0; i < 8; i++)
    {
        if (!world_to_screen(math::transform(points.at(i), transformed), screen.at(i)))
            return false;

        auto x = screen.at(i).x;
        auto y = screen.at(i).y;

        left   = std::min(left, x);
        top    = std::min(top, y);
        right  = std::max(right, x);
        bottom = std::max(bottom, y);
    }

    box.left   = left;
    box.top    = top;
    box.right  = right;
    box.bottom = bottom;
    box.width  = right - left;
    box.height = bottom - top;

    return true;
}

static void player_esp(cs_player* player, box_t& box)
{
    player_info_t info {};
    if (!game::interfaces.engine->get_player_info(player->networkable()->index(), &info))
        return;

    if (!player->alive())
        return;

    if (cheat::config.get<bool>("esp.player.box"))
    {
        auto color = cheat::config.get<color_t>("esp.player.box.clr");
        render::add_rect(ImVec2(box.left, box.top), ImVec2(box.right, box.bottom), ImColor(color.r, color.g, color.b, color.a));
    }

    if (cheat::config.get<bool>("esp.player.name"))
    {
        std::string name = info.name;
        if (name.length() > 18)
        {
            name = name.substr(0, 18);
            name.append("...");
        }
        const auto text_size = ImGui::CalcTextSize(name.c_str());
        render::add_text(ImVec2(box.left + box.width / 2 - text_size.x / 2, box.top - text_size.y - 2), ImColor(255, 255, 255), name.c_str());
    }

    if (cheat::config.get<bool>("esp.player.health"))
    {
        const auto health = player->health();
        const auto scalar = static_cast<int>(2.55f * static_cast<float>(health));
        const auto factor = static_cast<float>(health) / 100.f;

        render::add_rect_filled(ImVec2(box.left - 5 - 2, box.top), ImVec2(box.left - 5, box.bottom), ImColor(0, 0, 0));
        render::add_rect_filled(ImVec2(box.left - 5 - 2, box.bottom - (box.height * factor)), ImVec2(box.left - 5, box.bottom), ImColor(255 - scalar, scalar, 0));
    }
}

void features::esp::render()
{
    if (!game::interfaces.engine->in_game())
        return;

    if (!cheat::config.get<bool>("esp.enabled"))
        return;

    std::vector<base_entity*> list;
    for (auto i = 1; i < game::interfaces.entity_list->get_highest_entity_index(); i++)
    {
        auto ent = game::interfaces.entity_list->get<base_entity*>(i);
        if (!ent)
            continue;
        if (ent == cheat::local)
            continue;
        if (ent->networkable()->dormant())
            continue;
        list.emplace_back(ent);
    }

    for (auto& ent : list)
    {
        auto client_class = ent->networkable()->get_client_class();
        if (!client_class)
            continue;

        switch (client_class->m_class_id)
        {
            case e_class_ids::CCSPlayer:
            {
                box_t b {};
                if (!calculate_box(ent, b))
                    break;
                player_esp(reinterpret_cast<cs_player*>(ent), b);
                break;
            }
            default:
                break;
        }
    }
}