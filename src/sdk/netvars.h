#pragma once

#include <fstream>
#include <functional>
#include <type_traits>
#include <unordered_map>

struct netvar_t
{
    bool dump();
    bool dump_datamap();

    std::unordered_map<std::size_t, std::size_t> m_offsets {};
    bool m_dumped = false;

private:
    bool dump_datamap(datamap_t* map);
    bool dump_data_desc_map(datamap_t* map);

    void store(std::string_view name, recv_table* table, std::size_t offset = 0);
    void store_datamap(datamap_t* map, std::size_t offset = 0, bool embedded = false, const std::string& name = "");
    std::ofstream m_file;
};

namespace game
{
    inline netvar_t netvar;
}

#define NETVAR(func_type, func, netvar_name)                                                                                                                                                   \
    [[nodiscard]] std::add_lvalue_reference_t<func_type> func()                                                                                                                                \
    {                                                                                                                                                                                          \
        static auto offset = game::netvar.m_offsets[std::hash<std::string> {}(netvar_name)];                                                                                                   \
        return *(std::add_pointer_t<func_type>)(reinterpret_cast<uintptr_t>(this) + offset);                                                                                                   \
    }

#define NETVAR_OFFSET(func_type, func, netvar_name, off)                                                                                                                                       \
    [[nodiscard]] std::add_lvalue_reference_t<func_type> func()                                                                                                                                \
    {                                                                                                                                                                                          \
        static auto offset = game::netvar.m_offsets[std::hash<std::string> {}(netvar_name)];                                                                                                   \
        return *(std::add_pointer_t<func_type>)(reinterpret_cast<uintptr_t>(this) + offset + (off));                                                                                           \
    }
