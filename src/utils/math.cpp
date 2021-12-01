#include "../winter.h"
#include <random>
#include "math.h"

int math::random_int(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

float math::random_float(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

vec3_t math::transform(const vec3_t& tr, const matrix3x4_t& matrix)
{
    return { tr.dot(vec3_t(matrix[0])) + matrix[0][3], tr.dot(vec3_t(matrix[1])) + matrix[1][3], tr.dot(vec3_t(matrix[2])) + matrix[2][3] };
}

void math::transform(const vec3_t& tr, const matrix3x4_t& matrix, vec3_t& out)
{
    out.x = tr.dot(vec3_t(matrix[0])) + matrix[0][3];
    out.y = tr.dot(vec3_t(matrix[1])) + matrix[1][3];
    out.z = tr.dot(vec3_t(matrix[2])) + matrix[2][3];
}

float math::normalize(float yaw)
{
    return std::isfinite(yaw) ? std::remainder(yaw, 360.f) : 0.f;
}

void math::angle_vectors(const angle_t& ang, vec3_t& out_forward)
{
    auto sine = vec2_t(), cosine = vec2_t();

    sincos(deg2rad(ang.x), sine.x, cosine.x);
    sincos(deg2rad(ang.y), sine.y, cosine.y);

    out_forward.x = cosine.x * cosine.y;
    out_forward.y = cosine.x * sine.y;
    out_forward.z = -sine.x;
}

void math::angle_vectors(const angle_t& ang, vec3_t& out_forward, vec3_t& out_right, vec3_t& out_up)
{
    auto sine = vec3_t(), cosine = vec3_t();

    sincos(deg2rad(ang.x), sine.x, cosine.x);
    sincos(deg2rad(ang.y), sine.y, cosine.y);
    sincos(deg2rad(ang.z), sine.z, cosine.z);

    out_forward.x = cosine.x * cosine.y;
    out_forward.y = cosine.x * sine.y;
    out_forward.z = -sine.x;

    out_right.x = -1 * sine.z * sine.x * cosine.y + -1 * cosine.z * -sine.y;
    out_right.y = -1 * sine.z * sine.x * sine.y + -1 * cosine.z * cosine.y;
    out_right.z = -1 * sine.z * cosine.x;

    out_up.x = cosine.z * sine.x * cosine.y + -sine.z * -sine.y;
    out_up.y = cosine.z * sine.x * sine.y + -sine.z * cosine.y;
    out_up.z = cosine.z * cosine.x;
}

void math::vector_angles(const vec3_t& vec, angle_t& out)
{
    if (vec.x == 0.0f && vec.y == 0.0f)
    {
        out.x = (vec.z > 0.0f) ? 270.0f : 90.0f;
        out.y = 0.0f;
    }
    else
    {
        out.x = std::atan2(-vec.z, vec.length_2d()) * (-180.f / static_cast<float>(M_PI));
        out.y = std::atan2(vec.y, vec.x) * (180.f / static_cast<float>(M_PI));

        if (out.y > 90.f)
            out.y -= 180.f;
        else if (out.y < 90.f)
            out.y += 180.f;
        else if (out.y == 90.f)
            out.y = 0.f;
    }

    out.z = 0.f;
}

void math::vector_angles(const vec3_t& vec, const vec3_t& up, angle_t& out)
{
    const vec3_t left = cross_product(up, vec);

    if (const auto dist = vec.length_2d(); dist > 0.001f)
    {
        out.y = rad2deg(std::atan2(vec.y, vec.x));
        out.x = rad2deg(std::atan2(-vec.z, dist));
        out.z = rad2deg(std::atan2(left.z, left.y * vec.x - left.x * vec.y));
    }
    else
    {
        out.y = rad2deg(std::atan2(-left.x, left.y));
        out.x = rad2deg(std::atan2(-vec.z, dist));
        out.z = 0.f;
    }
}

// https://github.com/LWSS/Fuzion/blob/master/src/Utils/math.cpp#L97
void math::correct_movement(user_cmd* cmd, const angle_t& old_angle, float old_forward_move, float old_side_move)
{
    float f1         = normalize(old_angle.y);
    float f2         = normalize(cmd->view_angles.y);
    float delta_view = normalize(abs(f2 - f1));

    cmd->forward_move = cos(deg2rad(delta_view)) * old_forward_move + cos(deg2rad(delta_view + 90.f)) * old_side_move;
    cmd->side_move    = sin(deg2rad(delta_view)) * old_forward_move + sin(deg2rad(delta_view + 90.f)) * old_side_move;
}