//
// Created by deadwinter on 2021-11-05.
//
#include "../winter.h"
#include "string_helper.h"

std::vector<std::string> str_helper::split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> vec;
    size_t last = 0;
    size_t next;
    while ((next = str.find(delimiter, last)) != std::string::npos)
    {
        auto s = str.substr(last, next - last);
        if (!s.empty())
            vec.push_back(s);
        last = next + 1;
    }
    auto s = str.substr(last);
    if (!s.empty())
        vec.push_back(s);
    return vec;
}

std::string str_helper::lowercase(std::string& str)
{
    std::ranges::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });

    return str;
}
