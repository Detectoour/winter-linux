#include <string_view>
#include "../imgui/imgui.h"
#include "render.h"

void render::render()
{
    for (const auto i : draw_list)
    {
        switch (i.type)
        {
            case draw_type_t::TEXT:
            {
                if (i.font)
                {
                    ImGui::GetBackgroundDrawList()->PushTextureID(i.font->ContainerAtlas->TexID);
                    ImGui::GetBackgroundDrawList()->AddText(i.font, i.font_size, i.pos_1, i.clr, i.text);
                    ImGui::GetBackgroundDrawList()->PopTextureID();
                }
                else
                {
                    ImGui::GetBackgroundDrawList()->AddText(i.pos_1, i.clr, i.text);
                }
                break;
            }
            case draw_type_t::RECT:
            {
                ImGui::GetBackgroundDrawList()->AddRect(i.pos_1, i.pos_2, i.clr);
                break;
            }
            case draw_type_t::RECT_FILLED:
            {
                ImGui::GetBackgroundDrawList()->AddRectFilled(i.pos_1, i.pos_2, i.clr);
                break;
            }
            case draw_type_t::LINE:
            {
                ImGui::GetBackgroundDrawList()->AddLine(i.pos_1, i.pos_2, i.clr);
                break;
            }
            default:
                break;
        }
    }
    draw_list.clear();
}

void render::add_text(ImVec2 pos, ImColor clr, std::string_view text, float font_size, ImFont* font)
{
    draw_data_t data;
    data.type      = draw_type_t::TEXT;
    data.pos_1     = pos;
    data.clr       = clr;
    data.text      = text.data();
    data.font_size = font_size;
    data.font      = font;
    draw_list.emplace_back(data);
}

void render::add_line(ImVec2 pos_1, ImVec2 pos_2, ImColor clr)
{
    draw_data_t data;
    data.type  = draw_type_t::LINE;
    data.pos_1 = pos_1;
    data.pos_2 = pos_2;
    data.clr   = clr;
    draw_list.emplace_back(data);
}

void render::add_rect(ImVec2 pos_1, ImVec2 pos_2, ImColor clr)
{
    draw_data_t data;
    data.type  = draw_type_t::RECT;
    data.pos_1 = pos_1;
    data.pos_2 = pos_2;
    data.clr   = clr;
    draw_list.emplace_back(data);
}

void render::add_rect_filled(ImVec2 pos_1, ImVec2 pos_2, ImColor clr)
{
    draw_data_t data;
    data.type  = draw_type_t::RECT_FILLED;
    data.pos_1 = pos_1;
    data.pos_2 = pos_2;
    data.clr   = clr;
    draw_list.emplace_back(data);
}