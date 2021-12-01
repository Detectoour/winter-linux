#pragma once
#include <deque>

enum class draw_type_t : uint8_t
{
    LINE,
    RECT,
    RECT_FILLED,
    TEXT
};

struct draw_data_t
{
    draw_data_t() = default;
    draw_type_t type;
    ImVec2 pos_1 {};
    ImVec2 pos_2 {};
    ImColor clr {};
    const char* text {};
    float font_size;
    ImFont* font;
};

namespace render
{
    inline std::deque<draw_data_t> draw_list;
    void render();

    void add_line(ImVec2 pos_1, ImVec2 pos_2, ImColor clr);
    void add_text(ImVec2 pos, ImColor clr, std::string_view text, float font_size = 1.0f, ImFont* font = nullptr);
    void add_rect(ImVec2 pos_1, ImVec2 pos_2, ImColor clr);
    void add_rect_filled(ImVec2 pos_1, ImVec2 pos_2, ImColor clr);
} // namespace render