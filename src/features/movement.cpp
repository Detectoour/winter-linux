#include "../winter.h"
#include "movement.h"
#include "../config.h"
#include "../utils/math.h"
#include "../utils/strafer.h"

void features::movement::autobhop(cs_player* local, user_cmd* cmd)
{
    if (!cheat::config.get<bool>("movement.bunnyhop"))
        return;

    if (!local->alive() || local->move_type() == move_type_t::MOVETYPE_NOCLIP || local->move_type() == move_type_t::MOVETYPE_LADDER || local->water_level() >= water_level_t::WL_Waist)
        return;

    if (!local->ground_entity() && cmd->buttons.has_flag(IN_JUMP))
    {
        cmd->buttons.remove_flag(IN_JUMP);
    }
}

void features::movement::airstrafe(cs_player* local, user_cmd* cmd)
{
#define STRAFE_POWER 100.f
    if (!cheat::config.get<bool>("movement.autostrafe"))
        return;

    if (!local->alive() || local->move_type() == move_type_t::MOVETYPE_NOCLIP || local->move_type() == move_type_t::MOVETYPE_LADDER || local->water_level() >= water_level_t::WL_Waist)
        return;

    if (local->ground_entity())
        return;

    // TODO: Surf detection

    if (cmd->buttons.has_flag(IN_SPEED | IN_MOVELEFT | IN_MOVERIGHT))
        return;

    const auto max_speed        = local->max_speed();
    const auto surface_friction = local->surface_friction();
    const auto vel              = local->velocity().to_2d();
    const auto frame_time       = game::interfaces.global->interval_per_tick;
    const auto air_accelerate   = game::interfaces.convar->find_var("sv_airaccelerate")->get_float();
    const auto air_speedcap     = game::interfaces.convar->find_var("sv_air_max_wishspeed")->get_float();

    angle_t current_angle;
    game::interfaces.engine->get_view_angles(current_angle);
    current_angle.normalize();
    const auto yaw_diff = math::normalize(current_angle.y - last_yaw);
    const auto abs_diff = std::abs(yaw_diff);

    float forward_move, side_move;

    strafer::get_ideal_air_movement(current_angle.y, vel, air_accelerate, max_speed, air_speedcap, surface_friction, frame_time, forward_move, side_move, false);

    if (yaw_diff < 0.f)
    {
        const auto max_delta = strafer::get_air_max_delta(vel, air_accelerate, max_speed, air_speedcap, surface_friction, frame_time, false);
        cmd->side_move       = 450.f;
        if (abs_diff <= max_delta * STRAFE_POWER)
        {
            cmd->forward_move = forward_move * 450.f;
            cmd->side_move    = side_move * 450.f;
        }
    }
    else if (yaw_diff > 0.f)
    {
        const auto max_delta = strafer::get_air_max_delta(vel, air_accelerate, max_speed, air_speedcap, surface_friction, frame_time, true);
        cmd->side_move       = -450.f;
        if (abs_diff <= max_delta * STRAFE_POWER)
        {
            cmd->forward_move = forward_move * 450.f;
            cmd->side_move    = side_move * 450.f;
        }
    }
    else
    {
        cmd->forward_move = forward_move * 450.f;
        cmd->side_move    = side_move * 450.f;
    }

    last_yaw = current_angle.y;
}