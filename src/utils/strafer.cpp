#include "../winter.h"
#include "math.h"
#include "strafer.h"

// Credit goes to XutaxKamay <3
float strafer::vec_to_yaw(vec2_t vec)
{
    float yaw = 0.0;
    if (vec.x != 0.0f || vec.y != 0.0f)
    {
        vec2_t normalized_vec;
        normalized_vec.x = vec.x / vec.length();
        normalized_vec.y = vec.y / vec.length();

        yaw = std::atan2(normalized_vec.y, normalized_vec.x) * (180.0f / static_cast<float>(M_PI));
        return math::normalize(yaw);
    }
    return yaw;
}

void strafer::solve_2d_movement(vec2_t wish_dir, vec2_t forward, vec2_t right, float& forward_move, float& side_move)
{
    const auto v = wish_dir.x;
    const auto w = wish_dir.y;
    const auto a = forward.x;
    const auto c = right.x;
    const auto e = forward.y;
    const auto f = right.y;

    if (const auto divide = c * e - a * f; divide == 0.0f)
    {
        forward_move = 450.0f; // Change this to the default cl_sidespeed value if you want to port to other games
        side_move    = 0.0;
    }
    else
    {
        forward_move = (c * w - f * v) / divide;
        side_move    = (e * v - a * w) / divide;
    }
}

float strafer::get_air_theta_angle(vec2_t vel, float air_accelerate, float max_speed, float air_speed_cap, float surface_friction, float frame_time)
{
    const auto accel_speed = air_accelerate * max_speed * surface_friction * frame_time;

    if (const auto wanted_dot_product = air_speed_cap - accel_speed; wanted_dot_product > 0.0f)
    {
        const auto horizontal_speed = vel.length();
        if (horizontal_speed == 0.0f)
        {
            return 90.f;
        }

        auto cos_theta = wanted_dot_product / horizontal_speed;
        if (cos_theta > 1.0f)
        {
            cos_theta = 1.0f;
        }
        else if (cos_theta < -1.0f)
        {
            cos_theta = -1.0f;
        }

        const auto theta = std::acos(cos_theta) * (180.f / static_cast<float>(M_PI));
        return theta;
    }
    return 90.f;
}

float strafer::get_air_max_delta(vec2_t vel, float air_accelerate, float max_speed, float air_speed_cap, float surface_friction, float frame_time, bool left)
{
    const auto yaw_velocity = vec_to_yaw(vel);

    const auto theta = get_air_theta_angle(vel, air_accelerate, max_speed, air_speed_cap, surface_friction, frame_time);

    const auto best_yaw_right = math::normalize(yaw_velocity + theta);
    const auto best_yaw_left  = math::normalize(yaw_velocity - theta);

    vec3_t best_dir_left, best_dir_right;
    angle_t temp_angle { 0.f, 0.f, 0.f };

    temp_angle.y = best_yaw_right;
    math::angle_vectors(temp_angle, best_dir_right);

    temp_angle.y = best_yaw_left;
    math::angle_vectors(temp_angle, best_dir_left);

    const vec2_t best_right = best_dir_right.to_2d();
    const vec2_t best_left  = best_dir_left.to_2d();

    vec2_t calculated_left_velocity { 0.0f }, calculated_right_velocity { 0.0f };
    simulate_air_movement(vel, best_left, air_accelerate, air_speed_cap, max_speed, surface_friction, frame_time, calculated_left_velocity);
    simulate_air_movement(vel, best_right, air_accelerate, air_speed_cap, max_speed, surface_friction, frame_time, calculated_right_velocity);

    const auto new_best_yaw_left  = vec_to_yaw(calculated_left_velocity);
    const auto new_best_yaw_right = vec_to_yaw(calculated_right_velocity);

    return std::abs(math::normalize(yaw_velocity - (left ? new_best_yaw_left : new_best_yaw_right)));
}

void strafer::simulate_air_movement(vec2_t vel, vec2_t wish_dir, float air_accelerate, float air_speed_cap, float max_speed, float surface_friction, float frame_time, vec2_t& out_vel)
{
    auto wish_speed_cap = max_speed;
    if (wish_speed_cap > air_speed_cap)
        wish_speed_cap = air_speed_cap;

    const auto current_speed = vel.x * wish_dir.x + vel.y * wish_dir.y;
    const auto add_speed     = wish_speed_cap - current_speed;

    if (add_speed <= 0.0f)
        return;

    auto accel_speed = air_accelerate * max_speed * surface_friction * frame_time;

    if (accel_speed > add_speed)
        accel_speed = add_speed;

    out_vel.x = vel.x + accel_speed * wish_dir.x;
    out_vel.y = vel.y + accel_speed * wish_dir.y;
}

void strafer::get_ideal_air_movement(float wanted_yaw,
                                     vec2_t vel,
                                     float air_accelerate,
                                     float max_speed,
                                     float air_speed_cap,
                                     float surface_friction,
                                     float frame_time,
                                     float& forward_move,
                                     float& side_move,
                                     bool left)
{
    const auto yaw_velocity = vec_to_yaw(vel);

    const auto theta = get_air_theta_angle(vel, air_accelerate, max_speed, air_speed_cap, surface_friction, frame_time);

    const auto best_yaw_right = math::normalize(yaw_velocity + theta);
    const auto best_yaw_left  = math::normalize(yaw_velocity - theta);

    vec3_t best_dir_left, best_dir_right;
    angle_t temp_angle { 0.f, 0.f, 0.f };

    temp_angle.y = best_yaw_right;
    math::angle_vectors(temp_angle, best_dir_right);

    temp_angle.y = best_yaw_left;
    math::angle_vectors(temp_angle, best_dir_left);

    vec2_t best_dir;
    if (const auto diff_yaw = math::normalize(wanted_yaw - yaw_velocity); diff_yaw > 0.0f)
    {
        best_dir = best_dir_right.to_2d();
    }
    else if (diff_yaw < 0.0f)
    {
        best_dir = best_dir_left.to_2d();
    }
    else
    {
        best_dir = left ? best_dir_left.to_2d() : best_dir_right.to_2d();
    }

    vec3_t forward_wanted_dir_vec3, right_wanted_dir, up_wanted_dir;
    temp_angle.y = wanted_yaw;

    math::angle_vectors(temp_angle, forward_wanted_dir_vec3, right_wanted_dir, up_wanted_dir);

    solve_2d_movement(best_dir, forward_wanted_dir_vec3.to_2d(), right_wanted_dir.to_2d(), forward_move, side_move);

    if (const auto movement_length = std::sqrt(forward_move * forward_move + side_move * side_move); movement_length != 0.0f)
    {
        forward_move /= movement_length;
        side_move /= movement_length;
    }
}