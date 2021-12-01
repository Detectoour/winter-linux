#pragma once

// Could have gone with uint64_t but uint64_t means long long int to gcc
typedef unsigned long long font;

struct surface_t
{
    auto set_draw_color(int r, int g, int b, int a = 255)
    {
        return memory::get_vfunc<void (*)(void*, int, int, int, int)>(this, 14)(this, r, g, b, a);
    }

    auto set_draw_color(const color_t& clr)
    {
        set_draw_color(clr.r, clr.g, clr.b, clr.a);
    }

    auto draw_filled_rect(int x_0, int y_0, int x_1, int y_1)
    {
        return memory::get_vfunc<void (*)(void*, int, int, int, int)>(this, 16)(this, x_0, y_0, x_1, y_1);
    }

    auto draw_outlined_rect(int x_0, int y_0, int x_1, int y_1)
    {
        return memory::get_vfunc<void (*)(void*, int, int, int, int)>(this, 18)(this, x_0, y_0, x_1, y_1);
    }

    auto draw_line(int x_0, int y_0, int x_1, int y_1)
    {
        return memory::get_vfunc<void (*)(void*, int, int, int, int)>(this, 19)(this, x_0, y_0, x_1, y_1);
    }

    auto draw_poly_line(int* x, int* y, int count)
    {
        return memory::get_vfunc<void (*)(void*, int*, int*, int)>(this, 20)(this, x, y, count);
    }

    auto create_font()
    {
        return memory::get_vfunc<font (*)(void*)>(this, 71)(this);
    }

    auto set_font(font& f, const char* name, int tall, int weight, int blur, int scan_lines, int flags)
    {
        return memory::get_vfunc<void (*)(void*, font&, const char*, int, int, int, int, int)>(this, 72)(this, f, name, tall, weight, blur, scan_lines, flags);
    }

    auto get_text_size(font& f, const wchar_t* text, int& w, int& h)
    {
        return memory::get_vfunc<void (*)(void*, font&, const wchar_t*, int&, int&)>(this, 79)(this, f, text, w, h);
    }

    // search "in:  %5i  %5.2fk/s " in linux64/client_client.so
    // the sub_xxx function that the string passed in is Q_snprintf
    // the next line is DrawColoredText from surface
    // with this you can get rid of tons of functions (like set text
    // color, text pos, convert wstring to string etc.)
    auto draw_colored_text(font f, int x, int y, int r, int g, int b, int a, const char* sz)
    {
        return memory::get_vfunc<void (*)(void*, font, int, int, int, int, int, int, const char*)>(this, 163)(this, f, x, y, r, g, b, a, sz);
    }

    auto draw_colored_text(font& f, const vec2_t& pos, const color_t& clr, const std::string& sz)
    {
        draw_colored_text(f, static_cast<int>(pos.x), static_cast<int>(pos.y), clr.r, clr.g, clr.b, clr.a, sz.c_str());
    }
};