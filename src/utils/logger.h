#pragma once
#include <fmt/chrono.h>
#include <fstream>

enum class log_level
{
    info = 0,
    critical, // pretty much success, i don't know how to name it properly
    error
};

namespace logger
{
    inline std::ofstream m_file;
    void log(std::string str, log_level level = log_level::info, bool use_convar = true);

} // namespace logger