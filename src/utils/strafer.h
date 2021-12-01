#pragma once

namespace strafer
{
    float vec_to_yaw(vec2_t vec);
    void solve_2d_movement(vec2_t wish_dir, vec2_t forward, vec2_t right, float& forward_move, float& side_move);

    float get_air_theta_angle(vec2_t vel, float air_accelerate, float max_speed, float air_speed_cap, float surface_friction, float frame_time);
    float get_air_max_delta(vec2_t vel, float air_accelerate, float max_speed, float air_speed_cap, float surface_friction, float frame_time, bool left);
    void simulate_air_movement(vec2_t vel, vec2_t wish_dir, float air_accelerate, float air_speed_cap, float max_speed, float surface_friction, float frame_time, vec2_t& out_vel);
    void get_ideal_air_movement(float wanted_yaw,
                                vec2_t vel,
                                float air_accelerate,
                                float max_speed,
                                float air_speed_cap,
                                float surface_friction,
                                float frame_time,
                                float& forward_move,
                                float& side_move,
                                bool left);

}