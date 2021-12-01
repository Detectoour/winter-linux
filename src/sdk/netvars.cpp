#include "../winter.h"
#include "netvars.h"

// #define DUMP_DATA_PROP

bool netvar_t::dump()
{
    if (auto list = game::interfaces.base_client->get_classes())
    {
#ifdef DUMP_DATA_PROP
        m_file.open("/tmp/netvar.txt", std::ios::out | std::ios::trunc);
#endif
        while (list)
        {
            store(list->m_recv_table->m_net_table_name, list->m_recv_table);

            list = list->m_next;
        }
#ifdef DUMP_DATA_PROP
        m_file.close();
#endif
        return true;
    }
    else
    {
        return false;
    }
}

bool netvar_t::dump_datamap()
{
    if (auto local = game::interfaces.entity_list->get<cs_player*>(game::interfaces.engine->get_local_player()); local)
    {
        return m_dumped = dump_datamap(local->pred_datamap()) && dump_data_desc_map(local->data_desc_map());
    }
    return false;
}

void netvar_t::store(std::string_view name, recv_table* table, std::size_t offset)
{
#ifdef DUMP_DATA_PROP
    static auto type_to_string = [](const int& t)
    {
        switch (t)
        {
            case 0:
            default:
                return "int";
            case 1:
                return "float";
            case 2:
                return "vec3_t";
            case 3:
                return "vec2_t";
            case 4:
                return "const char*";
            case 5:
                return "array";
            case 6:
                return "data_table";
            case 7:
                return "int64_t";
        }
    };
#endif

    for (auto i = 0; i < table->m_n_props; ++i)
    {
        const auto prop = &table->m_props[i];

        if (const auto child = prop->m_data_table; child && child->m_n_props > 0)
            store(name, child, prop->m_offset + offset);

        const auto var_name = prop->name;
        auto netvar         = std::string(name.data()) + "->" + var_name;
        auto hashed         = std::hash<std::string> {}(netvar);
#ifdef DUMP_DATA_PROP
        m_file << "NETVAR( " << type_to_string(prop->type) << ", " << var_name << ", \"" << netvar << "\" ) // "
               << "0x" << std::hex << std::uppercase << prop->m_offset + offset << std::dec << std::endl;
#endif
        if (!m_offsets[hashed] && prop->m_offset)
        {
            m_offsets[hashed] = prop->m_offset + offset;
        }
    }
}

void netvar_t::store_datamap(datamap_t* map, std::size_t offset, bool embedded, const std::string& name)
{
    if (!map)
    {
        return;
    }
#ifdef DUMP_DATA_PROP
    static auto type_to_string = [](const field_types& t)
    {
        switch (t)
        {
            case FIELD_VOID:
            default:
                return "void";
            case FIELD_FLOAT:
                return "float";
            case FIELD_STRING:
            case FIELD_MODELNAME:
            case FIELD_SOUNDNAME:
                return "const char*";
            case FIELD_VECTOR:
            case FIELD_POSITION_VECTOR:
                return "vec3_t";
            case FIELD_QUATERNION:
                return "vec4_t";
            case FIELD_INTEGER:
                return "int";
            case FIELD_BOOLEAN:
                return "bool";
            case FIELD_SHORT:
                return "short";
            case FIELD_CHARACTER:
                return "char";
            case FIELD_COLOR32:
                return "color_t";
            case FIELD_VECTOR2D:
                return "vec2_t";
            case FIELD_MODELINDEX:
            case FIELD_MATERIALINDEX:
            case FIELD_EHANDLE:
                return "size_t";
            case FIELD_VMATRIX:
            case FIELD_VMATRIX_WORLDSPACE:
            case FIELD_MATRIX3X4_WORLDSPACE:
                return "matrix3x4_t";
        }
    };
#endif
    for (int i = 0; i < map->data_num_fields; ++i)
    {
        const auto field = &map->data_desc[i];

        if (!memory::is_valid_address(reinterpret_cast<uintptr_t>(field->field_name)))
        {
            continue;
        }

        std::string class_name = "DT_" + std::string(map->data_class_name).substr(2);

        std::string datamap = embedded ? std::string(name) + "->" + field->field_name : class_name + "->" + field->field_name;
        ;

        if (field->field_type == FIELD_EMBEDDED)
        {
            store_datamap(field->td, field->field_offset[0] + offset, true, class_name);
        }
        auto hashed = std::hash<std::string> {}(datamap);
#ifdef DUMP_DATA_PROP
        m_file << "NETVAR( " << type_to_string(field->field_type) << ", " << field->field_name << ", \"" << datamap << "\" ) // "
               << "0x" << std::hex << std::uppercase << field->field_offset[0] + offset << std::dec << std::endl;
#endif
        if (!m_offsets[hashed] && field->field_offset[0])
        {
            m_offsets[hashed] = field->field_offset[0] + offset;
        }
    }
}

bool netvar_t::dump_datamap(datamap_t* map)
{
    if (!map)
    {
        return false;
    }
#ifdef DUMP_DATA_PROP
    m_file.open("/tmp/datamap.txt", std::ios::out | std::ios::trunc);
#endif
    while (map)
    {
        store_datamap(map);
        map = map->base_map;
    }
#ifdef DUMP_DATA_PROP
    m_file.close();
#endif
    logger::log("Finished dumping PredDatamap");
    return true;
}

bool netvar_t::dump_data_desc_map(datamap_t* map)
{
    if (!map)
    {
        return false;
    }
#ifdef DUMP_DATA_PROP
    m_file.open("/tmp/data_desc_map.txt", std::ios::out | std::ios::trunc);
#endif
    while (map)
    {
        store_datamap(map);
        map = map->base_map;
    }
#ifdef DUMP_DATA_PROP
    m_file.close();
#endif
    logger::log("Finished dumping DatadescMap");

    return true;
}
