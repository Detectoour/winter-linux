#pragma once

namespace math
{
    int random_int(int min, int max);
    float random_float(float min, float max);

    vec3_t transform(const vec3_t& tr, const matrix3x4_t& matrix);
    void transform(const vec3_t& tr, const matrix3x4_t& matrix, vec3_t& out);
    float normalize(float yaw);
    void angle_vectors(const angle_t& ang, vec3_t& out_forward);
    void angle_vectors(const angle_t& ang, vec3_t& out_forward, vec3_t& out_right, vec3_t& out_up);
    void vector_angles(const vec3_t& vec, angle_t& out);
    void vector_angles(const vec3_t& vec, const vec3_t& up, angle_t& out);
    void correct_movement(user_cmd* cmd, const angle_t& old_angle, float old_forward_move, float old_side_move);

    inline float rad2deg(const float rad)
    {
        return static_cast<float>(rad * (180.f / M_PI));
    }

    inline float deg2rad(const float deg)
    {
        return static_cast<float>(deg * (M_PI / 180.f));
    }

    inline void sincos(const float rad, float& sine, float& cosine)
    {
        sine   = std::sin(rad);
        cosine = std::cos(rad);
    }

    inline vec3_t cross_product(const vec3_t& a, const vec3_t& b)
    {
        return vec3_t(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

}; // namespace math