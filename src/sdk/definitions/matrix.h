#pragma once

struct matrix3x4_t
{
    matrix3x4_t() = default;

    matrix3x4_t(float m_00, float m_01, float m_02, float m_03, float m_10, float m_11, float m_12, float m_13, float m_20, float m_21, float m_22, float m_23)
    {
        m_matrix[0][0] = m_00;
        m_matrix[0][1] = m_01;
        m_matrix[0][2] = m_02;
        m_matrix[0][3] = m_03;
        m_matrix[1][0] = m_10;
        m_matrix[1][1] = m_11;
        m_matrix[1][2] = m_12;
        m_matrix[1][3] = m_13;
        m_matrix[2][0] = m_20;
        m_matrix[2][1] = m_21;
        m_matrix[2][2] = m_22;
        m_matrix[2][3] = m_23;
    }

    matrix3x4_t(const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& vec_origin)
    {
        init(x_axis, y_axis, z_axis, vec_origin);
    }

    void init(const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& vec_origin)
    {
        m_matrix[0][0] = x_axis.x;
        m_matrix[0][1] = y_axis.x;
        m_matrix[0][2] = z_axis.x;
        m_matrix[0][3] = vec_origin.x;
        m_matrix[1][0] = x_axis.y;
        m_matrix[1][1] = y_axis.y;
        m_matrix[1][2] = z_axis.y;
        m_matrix[1][3] = vec_origin.y;
        m_matrix[2][0] = x_axis.z;
        m_matrix[2][1] = y_axis.z;
        m_matrix[2][2] = z_axis.z;
        m_matrix[2][3] = vec_origin.z;
    }

    float* operator[](int i)
    {
        return m_matrix[i];
    }

    const float* operator[](int i) const
    {
        return m_matrix[i];
    }

    float m_matrix[3][4] {};
};

struct matrix4x4_t
{
public:
    matrix4x4_t()
    {
    }
    matrix4x4_t(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
    {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;

        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;

        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;

        m[3][0] = m30;
        m[3][1] = m31;
        m[3][2] = m32;
        m[3][3] = m33;
    }

    // array access
    inline float* operator[](int i)
    {
        return m[i];
    }

    inline const float* operator[](int i) const
    {
        return m[i];
    }

    // Get a pointer to m[0][0]
    inline float* base()
    {
        return &m[0][0];
    }

    inline const float* base() const
    {
        return &m[0][0];
    }

public:
    // The matrix.
    float m[4][4];
};