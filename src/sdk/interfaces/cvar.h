#pragma once

#define FCVAR_NONE 0

#define FCVAR_UNREGISTERED    (1 << 0)
#define FCVAR_DEVELOPMENTONLY (1 << 1)
#define FCVAR_GAMEDLL         (1 << 2)
#define FCVAR_CLIENTDLL       (1 << 3)
#define FCVAR_HIDDEN          (1 << 4)

#define FCVAR_PROTECTED (1 << 5)
#define FCVAR_SPONLY    (1 << 6)
#define FCVAR_ARCHIVE   (1 << 7)
#define FCVAR_NOTIFY    (1 << 8)
#define FCVAR_USERINFO  (1 << 9)
#define FCVAR_CHEAT     (1 << 14)

#define FCVAR_PRINTABLEONLY   (1 << 10)
#define FCVAR_UNLOGGED        (1 << 11)
#define FCVAR_NEVER_AS_STRING (1 << 12)
#define FCVAR_RELEASE         (1 << 19)

#define FCVAR_REPLICATED (1 << 13)
#define FCVAR_DEMO       (1 << 16)
#define FCVAR_DONTRECORD (1 << 17)

#define FCVAR_NOT_CONNECTED         (1 << 22)
#define FCVAR_ARCHIVE_XBOX          (1 << 24)
#define FCVAR_SERVER_CAN_EXECUTE    (1 << 28)
#define FCVAR_SERVER_CANNOT_QUERY   (1 << 29)
#define FCVAR_CLIENTCMD_CAN_EXECUTE (1 << 30)

class con_var
{
public:
    auto get_float()
    {
        return memory::get_vfunc<float (*)(void*)>(this, 15)(this);
    }
    auto get_int()
    {
        return memory::get_vfunc<int (*)(void*)>(this, 16)(this);
    }
    auto set_value(const char* value)
    {
        return memory::get_vfunc<void (*)(void*, const char*)>(this, 17)(this, value);
    }
    auto set_value(float value)
    {
        return memory::get_vfunc<void (*)(void*, float)>(this, 18)(this, value);
    }
    auto set_value(int value)
    {
        return memory::get_vfunc<void (*)(void*, int)>(this, 19)(this, value);
    }

    void* vtable;
    con_var* next;      // 0x0008
    bool is_registered; // 0x0009
    char pad_0011[7];   // 0x0010
    char* name;         // 0x0018
    char* description;  // 0x0020
    int32_t flags;      // 0x0028
    char pad_0018[0x4]; // 0x002C
    void* accessor;     // 0x0030
    con_var* parent;    // 0x0038
    char* str_default;  // 0x0040
    char* str_value;    // 0x0048
    int32_t str_length;
    int type;        // 0x0050
    float f_value;   // 0x0054
    int32_t i_value; // 0x0058
    bool has_min;    // 0x005C
    char pad_min[3];
    float min_val; // 0x0060
    bool has_max;  // 0x0064
    char pad_max[3];
    float max_val; // 0x0068
    char unk_pad[4];
    void* some_other_class;
    void* fn_change_callback;
};

struct cvar_t
{
    auto find_var(const char* value)
    {
        return memory::get_vfunc<con_var* (*)(void*, const char*)>(this, 15)(this, value);
    }

    template <typename... values>
    auto print_to_console(const color_t& clr, const char* fmt, values... parameters)
    {
        return memory::get_vfunc<void (*)(void*, const color_t&, const char*, ...)>(this, 25)(this, clr, fmt, parameters...);
    }

    auto print_to_console(const color_t& clr, std::string str)
    {
        print_to_console(clr, str.append("\n").c_str());
    }
};