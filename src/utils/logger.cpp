#include "../winter.h"

void logger::log(std::string str, log_level level, bool use_convar)
{
    std::string prefix;
    color_t clr;
    switch (level)
    {
        case log_level::info:
        default:
        {
            clr    = color_t(0, 255, 255, 255);
            prefix = "[-] ";
            break;
        }
        case log_level::critical:
        {
            clr    = color_t(0, 255, 0, 255);
            prefix = "[+] ";
            break;
        }
        case log_level::error:
        {
            clr    = color_t(255, 0, 0, 255);
            prefix = "[!] ";
            break;
        }
    }
    str = prefix + str + "\n";
    if (use_convar && game::interfaces.convar)
        game::interfaces.convar->print_to_console(clr, str.c_str());

    if (m_file.good())
    {
        auto time = std::time(nullptr);
        m_file << fmt::format("[{:%H:%M:%S}] {}", fmt::localtime(time), str);

        // TODO(?): Switch to spdlog, because it can flush the file every
        // x seconds
        m_file.flush();
    }
}