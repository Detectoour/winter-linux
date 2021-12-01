#include "../winter.h"

#include "../imgui/GL/gl3w.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_sdl.h"

#include "font.h"
#include "menu.h"

#include "../utils/render.h"
#include "../utils/string_helper.h"
#include <iterator>

#include "../features/esp.h"

int hook_funcs::sdl::poll_event(SDL_Event* event)
{
    const auto ret = cheat::menu.poll_event_original(event);
    cheat::menu.poll_event(event, ret);
    return ret;
}

void hook_funcs::sdl::swap_window(SDL_Window* window)
{
    cheat::menu.initialize(window);
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);

        ImGuiIO& io = ImGui::GetIO();
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        cheat::menu.m_screen_size = vec2_t { w, h };
        io.DisplaySize            = ImVec2(static_cast<float>(w), static_cast<float>(h));

        ImGui::NewFrame();
        {
            io.MouseDrawCursor = cheat::menu.m_open && game::interfaces.engine->in_game();
            cheat::menu.draw();
            features::esp::render();
            render::add_text(ImVec2(1, 1), ImColor(255, 255, 255), "Yomama deeznutz");
            render::render();
        }
        if (ImGui::IsKeyPressed(SDL_SCANCODE_INSERT, false))
        {
            cheat::menu.m_open = !cheat::menu.m_open;
        }
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    cheat::menu.swap_window_original(window);
}

void menu_t::hook()
{
    const auto sdl      = dlopen("libSDL2-2.0.so.0", RTLD_LAZY | RTLD_NOLOAD);
    swap_window_address = address_t(dlsym(sdl, "SDL_GL_SwapWindow")).add(2).relative(0x0);
    if (!memory::is_valid_address(swap_window_address))
    {
        logger::log("Invalid swap_window_address", log_level::error);
        return;
    }
    swap_window_original                                                                   = swap_window_address.get().cast<decltype(swap_window_original)>();
    *reinterpret_cast<decltype(hook_funcs::sdl::swap_window)**>(swap_window_address.m_ptr) = hook_funcs::sdl::swap_window;

    poll_event_address = address_t(dlsym(sdl, "SDL_PollEvent")).add(2).relative(0x0);
    if (!memory::is_valid_address(poll_event_address))
    {
        logger::log("Invalid poll_event_address", log_level::error);
        return;
    }

    poll_event_original                                                                  = poll_event_address.get().cast<decltype(poll_event_original)>();
    *reinterpret_cast<decltype(hook_funcs::sdl::poll_event)**>(poll_event_address.m_ptr) = hook_funcs::sdl::poll_event;

    logger::log("Finished SDL Hooking", log_level::critical);
}

void menu_t::unhook()
{
    if (!memory::is_valid_address(swap_window_address) || !memory::is_valid_address(poll_event_address))
        return;

    *reinterpret_cast<decltype(swap_window_original)*>(swap_window_address.m_ptr) = swap_window_original;
    *reinterpret_cast<decltype(poll_event_original)*>(poll_event_address.m_ptr)   = poll_event_original;
}

void menu_t::initialize(SDL_Window* window)
{
    if (m_initialized)
        return;

    gl3wInit();
    ImGui::CreateContext();
    {
        ImGuiStyle& style                            = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
        style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
        style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
        style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
        style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        style.GrabRounding = style.FrameRounding = 2.3f;
        style.WindowRounding                     = 0.f;
    }
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryCompressedTTF(font_compressed_data, font_compressed_size, 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    m_initialized = true;
    logger::log("Initialized ImGui", log_level::critical);
}

void menu_t::draw()
{
    if (!cheat::menu.m_open)
        return;

    console.draw(m_screen_size.x, m_screen_size.y);
}

void menu_t::poll_event(SDL_Event* event, int ret) const
{
    if (m_initialized && ret && ImGui_ImplSDL2_ProcessEvent(event) && m_open)
    {
        event->type = 0;
    }
}

void menu_t::console_t::add_log(const std::string& input)
{
    m_items.push_back(input + "\n");
}

void menu_t::console_t::clear_log()
{
    m_items.clear();
}

void menu_t::console_t::draw(float width, float height)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height / 4.f));

    ImGui::Begin("##winter", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);
    {
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

        ImGui::BeginChild("text_area", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
            for (const auto& i : m_items)
            {
                if (i.starts_with("> "))
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.f, 0.1f, 1.f));
                    ImGui::TextUnformatted("> ");
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::TextUnformatted(i.substr(2).c_str());
                }
                else if (i.starts_with("error: "))
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                    ImGui::TextUnformatted(i.c_str());
                    ImGui::PopStyleColor();
                }
                else
                {
                    ImGui::TextUnformatted(i.c_str());
                }
            }
            ImGui::PopStyleVar();
            if (m_scroll_to_bottom)
            {
                ImGui::SetScrollHereY(1.0f);
                m_scroll_to_bottom = false;
            }
        }
        ImGui::EndChild();
        ImGui::Separator();
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
        auto time_text                       = fmt::format("{:%H:%M:%S}", fmt::localtime(m_last_execute_time));
        auto text_size                       = ImGui::CalcTextSize(time_text.c_str(), nullptr, true);
        ImGui::SetNextItemWidth(width - ImGui::CalcTextSize("99:99:99", nullptr, true).x * 1.4f);

        auto focused = false;
        if (ImGui::InputText("##Input", m_input_str, 256, input_text_flags))
        {
            std::string str = m_input_str;
            execute_command(str);
            m_input_str[0] = '\0';
            focused        = true;
        }
        ImGui::SetItemDefaultFocus();
        if (focused)
            ImGui::SetKeyboardFocusHere(-1);
        ImGui::SameLine();
        ImGui::SetNextWindowPos(ImVec2(width - text_size.x * 0.5f, height - text_size.y * 0.5f), ImGuiCond_Always);
        ImGui::Text("%s", time_text.c_str());
    }
    ImGui::End();
}

#include "../config.h"

void menu_t::console_t::execute_command(const std::string& cmd)
{
    add_log("> " + cmd);
    m_last_execute_time = std::time(nullptr);
    m_scroll_to_bottom  = true;
    auto cmds           = str_helper::split(cmd, " ");
    if (cmds.empty())
    {
        return;
    }

    if (cmds[0] == "clear")
    {
        clear_log();
    }
    else if (cmds[0] == "help")
    {
        add_log("command list:");
        add_log("    clear");
        for (const auto& i : cheat::config.config_list)
            add_log(fmt::format("    {}", i.first));
    }
    else
    {
        if (cheat::config.config_list.contains(cmds[0]))
        {
            auto& it = cheat::config.config_list[cmds[0]];
            if (cmds.size() == 1)
            {
                if (std::holds_alternative<bool>(it))
                    add_log(fmt::format("{}: {}", cmds[0], std::get<bool>(it)));
                else if (std::holds_alternative<float>(it))
                    add_log(fmt::format("{}: {}", cmds[0], std::get<float>(it)));
                else if (std::holds_alternative<int>(it))
                    add_log(fmt::format("{}: {}", cmds[0], std::get<int>(it)));
                else if (std::holds_alternative<color_t>(it))
                {
                    auto clr = std::get<color_t>(it);
                    add_log(fmt::format("{}: R:{} G:{} B:{} A:{}", cmds[0], clr.r, clr.g, clr.b, clr.a));
                }
            }
            else if (cmds.size() >= 2)
            {
                try
                {
                    if (std::holds_alternative<bool>(it))
                    {
                        auto lowercase = str_helper::lowercase(cmds[1]);
                        if (lowercase != "true" && lowercase != "false")
                        {
                            auto v = std::clamp(std::stoi(cmds[1]), 0, 1);
                            it     = v == 0 ? false : true;
                        }
                        else
                        {
                            it = lowercase == "false" ? false : true;
                        }
                        add_log(fmt::format("{} ==> {}", cmds[0], std::get<bool>(it)));
                    }
                    else if (std::holds_alternative<float>(it))
                    {
                        it = std::stof(cmds[1]);
                        add_log(fmt::format("{} ==> {}", cmds[0], std::get<float>(it)));
                    }
                    else if (std::holds_alternative<int>(it))
                    {
                        it = std::stoi(cmds[1]);
                        add_log(fmt::format("{} ==> {}", cmds[0], std::get<int>(it)));
                    }
                    else if (std::holds_alternative<color_t>(it))
                    {
                        auto clr = std::get<color_t>(it);
                        clr.r    = std::stoi(cmds[1]);
                        // I am so sorry because I can't really think of a clean way to do this
                        if (cmds.size() == 3)
                        {
                            clr.g = std::stoi(cmds[2]);
                        }
                        else if (cmds.size() == 4)
                        {
                            clr.g = std::stoi(cmds[2]);
                            clr.b = std::stoi(cmds[3]);
                        }
                        else if (cmds.size() >= 5)
                        {
                            clr.g = std::stoi(cmds[2]);
                            clr.b = std::stoi(cmds[3]);
                            clr.a = std::stoi(cmds[4]);
                        }

                        it  = clr;
                        clr = std::get<color_t>(it);
                        add_log(fmt::format("{} ==> R:{} G:{} B:{} A:{}", cmds[0], clr.r, clr.g, clr.b, clr.a));
                    }
                    if (cheat::config.callback_list.contains(cmds[0]))
                    {
                        auto callback = cheat::config.callback_list[cmds[0]];
                        if (callback)
                            callback();
                    }
                }
                catch (std::exception& e)
                {
                    add_log(fmt::format("error: {}", e.what()));
                }
            }
        }
        else
        {
            add_log(fmt::format("error: unknown command \"{}\"", cmd));
        }
    }
}
