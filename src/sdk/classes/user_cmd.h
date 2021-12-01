#pragma once

struct user_cmd
{
    virtual ~user_cmd() = default;
    ;
    int command_number;
    int tick_count;
    angle_t view_angles;
    angle_t aim_direction;
    float forward_move;
    float side_move;
    float up_move;
    bitflag_t buttons;
    unsigned char impulse;
    int weapon_select;
    int weapon_subtype;
    int random_seed;
    short moused_x;
    short moused_y;
    bool has_been_predicted;
    angle_t head_angles;
    vec3_t head_offset;
};