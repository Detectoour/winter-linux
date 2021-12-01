#pragma once

namespace hook_funcs::sdl
{
    int poll_event(SDL_Event* event);

    void swap_window(SDL_Window* window);
} // namespace hook_funcs::sdl

class menu_t
{
private:
    struct console_t
    {
        time_t m_last_execute_time;
        char m_input_str[256];
        std::vector<std::string> m_items;
        std::vector<std::string> m_commands;
        bool m_scroll_to_bottom;

        static int stricmp(const char* s1, const char* s2)
        {
            int d;
            while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
            {
                s1++;
                s2++;
            }
            return d;
        }
        static int strnicmp(const char* s1, const char* s2, int n)
        {
            int d = 0;
            while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
            {
                s1++;
                s2++;
                n--;
            }
            return d;
        }
        static char* strdup(const char* s)
        {
            size_t len = strlen(s) + 1;
            void* buf  = malloc(len);
            return (char*)memcpy(buf, (const void*)s, len);
        }
        static void strtrim(char* s)
        {
            char* str_end = s + strlen(s);
            while (str_end > s && str_end[-1] == ' ')
                str_end--;
            *str_end = 0;
        }
        console_t()
        {
            memset(m_input_str, 0, sizeof(m_input_str));
            m_commands          = { "clear", "esp.enabled", "movement.bunnyhop", "test_command" };
            m_last_execute_time = std::time(nullptr);
        }

        void add_log(const std::string& input);
        void clear_log();
        void execute_command(const std::string& cmd);
        void draw(float width, float height);
    };
    console_t console;

public:
    void hook();
    void unhook();
    void initialize(SDL_Window* window);
    void draw();
    void poll_event(SDL_Event* event, int ret) const;

    bool m_open = false;
    std::add_pointer_t<void(SDL_Window*)> swap_window_original {};
    std::add_pointer_t<int(SDL_Event*)> poll_event_original {};
    vec2_t m_screen_size;

private:
    address_t swap_window_address;
    address_t poll_event_address;

    bool m_initialized = false;
};

namespace cheat
{
    inline menu_t menu;
}