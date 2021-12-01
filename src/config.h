#pragma once

#include <unordered_map>
#include <variant>

struct config_t
{
    config_t() = default;

    using types    = std::variant<bool, float, color_t, int>;
    using callback = std::function<bool()>;

    static void initialize();

    void add(const std::string& name, types default_value);
    void add_callback(const std::string& name, callback callback);

    template <typename T>
    T& get(const std::string& name)
    {
        return std::get<T>(config_list[name]);
    }

    callback get_callback(const std::string& name)
    {
        return callback_list[name];
    }

    std::unordered_map<std::string, types> config_list {};
    std::unordered_map<std::string, callback> callback_list {};
};

namespace cheat
{
    inline config_t config;
}