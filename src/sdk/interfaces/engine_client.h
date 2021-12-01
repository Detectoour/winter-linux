#pragma once

struct player_info_t
{
    int64_t pad_0;
    union
    {
        int64_t xuid;
        struct
        {
            int xuid_low;
            int xuid_high;
        };
    };
    char name[128];
    int userid;
    char guid[33];
    unsigned int friends_id;
    char friends_name[128];
    bool fake_player;
    bool is_hltv;
    unsigned int custom_files[4];
    unsigned char files_downloaded;
};

struct engine_client_t
{
    auto get_screen_size(int& w, int& h)
    {
        return memory::get_vfunc<void (*)(void*, int&, int&)>(this, 5)(this, w, h);
    }

    auto get_player_info(int idx, player_info_t* info)
    {
        return memory::get_vfunc<bool (*)(void*, int, player_info_t*)>(this, 8)(this, idx, info);
    }

    auto get_local_player()
    {
        return memory::get_vfunc<int (*)(void*)>(this, 12)(this);
    }

    auto get_view_angles(angle_t& angle)
    {
        return memory::get_vfunc<void (*)(void*, angle_t&)>(this, 18)(this, angle);
    }

    auto set_view_angles(angle_t& angle)
    {
        return memory::get_vfunc<void (*)(void*, angle_t&)>(this, 19)(this, angle);
    }

    auto in_game()
    {
        return memory::get_vfunc<bool (*)(void*)>(this, 26)(this);
    }

    auto world_to_screen_matrix()
    {
        return memory::get_vfunc<const matrix4x4_t& (*)(void*)>(this, 37)(this);
    }
};