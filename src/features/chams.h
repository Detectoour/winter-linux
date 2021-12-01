#pragma once

namespace features::chams
{
    bool draw_model_pre(void* ptr, void* result, const draw_model_info_t& info, matrix3x4_t* bone_to_world, float* weight, float* delayed_weight, const vec3_t* origin, int flags);
    bool draw_model_post(void* ptr, void* result, const draw_model_info_t& info, matrix3x4_t* bone_to_world, float* weight, float* delayed_weight, const vec3_t* origin, int flags);
    void initialize();
} // namespace features::chams