#pragma once

struct hud_chat_t
{
    void print_to_chat(int index, int filter, const std::string& msg, ...)
    {
        return memory::get_vfunc<void (*)(void*, int, int, const char*)>(this, 31)(this, index, filter, fmt::format("\x01 {}", msg).c_str());
    }

    void print_to_chat(const std::string& msg)
    {
        print_to_chat(0, 0, msg.data());
    }

    void print_to_chat(int index, const std::string& msg)
    {
        print_to_chat(index, 0, msg.data());
    }
};