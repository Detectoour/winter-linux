#pragma once

#include <vector>

namespace str_helper
{
    std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    std::string lowercase(std::string& str);
} // namespace str_helper