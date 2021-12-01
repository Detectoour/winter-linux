#pragma once

struct recv_prop;

struct variant
{
    union
    {
        float m_float;
        long m_int;
        char* m_string;
        void* m_data;
        float m_vector[3];
        int64_t m_int_64;
    };

    int m_type;
};

struct recv_proxy_data
{
    const recv_prop* m_p_recv_prop;
    variant m_value;
    int m_element;
    int m_object_id;
};

struct recv_table
{
    recv_prop* m_props;
    int m_n_props;
    void* m_decoder;
    char* m_net_table_name;
    bool m_initialized;
    bool m_in_main_list;
};

typedef void (*recv_var_proxy_fn)(const recv_proxy_data* data, void* struct_, void* out);

struct recv_prop
{
    char* name;
    int type;
    int flags;
    int m_string_buffer_size;
    bool m_inside_array;
    const void* m_extra_data;
    recv_prop* m_array_prop;
    void* m_array_length_proxy;
    recv_var_proxy_fn m_proxy_fn;
    void* m_data_table_proxy_fn;
    recv_table* m_data_table;
    int m_offset;
    int m_element_stride;
    int m_elements;
    const char* m_parent_array_prop_name;
};

typedef void* (*create_client_class_fn)(int ent_num, int serial_num);
typedef void* (*create_event_fn)();

class client_class
{
public:
    create_client_class_fn m_create_fn;
    create_event_fn* m_create_event_fn;
    char* m_network_name;
    recv_table* m_recv_table;
    client_class* m_next;
    e_class_ids m_class_id;
};

enum field_types
{
    FIELD_VOID = 0,
    FIELD_FLOAT,
    FIELD_STRING,
    FIELD_VECTOR,
    FIELD_QUATERNION,
    FIELD_INTEGER,
    FIELD_BOOLEAN,
    FIELD_SHORT,
    FIELD_CHARACTER,
    FIELD_COLOR32,
    FIELD_EMBEDDED,
    FIELD_CUSTOM,
    FIELD_CLASSPTR,
    FIELD_EHANDLE,
    FIELD_EDICT,
    FIELD_POSITION_VECTOR,
    FIELD_TIME,
    FIELD_TICK,
    FIELD_MODELNAME,
    FIELD_SOUNDNAME,
    FIELD_INPUT,
    FIELD_FUNCTION,
    FIELD_VMATRIX,
    FIELD_VMATRIX_WORLDSPACE,
    FIELD_MATRIX3X4_WORLDSPACE,
    FIELD_INTERVAL,
    FIELD_MODELINDEX,
    FIELD_MATERIALINDEX,
    FIELD_VECTOR2D,
    FIELD_TYPECOUNT
};

enum
{
    TD_OFFSET_NORMAL = 0,
    TD_OFFSET_PACKED,
    TD_OFFSET_COUNT
};

struct inputdata_t;
typedef void (*input_func_t)(inputdata_t& data);

struct datamap_t;
// https://github.com/NeKzor/SourceAutoRecord/blob/master/src/Utils/SDK.hpp#L670-L687
// The actual size is 104(dec), you can find it from here:
// client_client.so 48 8D 14 40 4C 8D 24 90 49 C1 E4 03 0F 1F 44 00 ? 48
// 89 DA 49 03 17 F6 42 17 80
struct type_description_t
{
    field_types field_type;
    const char* field_name;
    int field_offset[TD_OFFSET_COUNT];
    unsigned short field_size;
    short flags;
    const char* external_name;
    void* p_save_restore_ops;
    input_func_t input_func;
    datamap_t* td;
    int field_size_in_bytes;
    struct type_description_t* override_field;
    int override_count;
    float field_tolerance;
    char pad[16];
};

struct datamap_t
{
    type_description_t* data_desc;
    int data_num_fields;
    char const* data_class_name;
    datamap_t* base_map;
    bool chains_validated;
    bool packed_offsets_computed;
    int packed_size;
};