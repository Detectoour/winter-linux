#pragma once
#include <cfloat>

struct vec2_t
{
    vec2_t() : x {}, y {}
    {
    }

    explicit vec2_t(float t) : x { t }, y { t }
    {
    }

    explicit vec2_t(int t) : x { static_cast<float>(t) }, y { static_cast<float>(t) }
    {
    }

    vec2_t(int x, int y) : x { static_cast<float>(x) }, y { static_cast<float>(y) }
    {
    }

    vec2_t(float x, float y) : x { x }, y { y }
    {
    }

    vec2_t& operator+=(const vec2_t& v)
    {
        x += v.x;
        y += v.y;

        return *this;
    }

    vec2_t& operator-=(const vec2_t& v)
    {
        x -= v.x;
        y -= v.y;

        return *this;
    }

    vec2_t& operator*=(const vec2_t& v)
    {
        x *= v.x;
        y *= v.y;

        return *this;
    }

    vec2_t& operator/=(const vec2_t& v)
    {
        x /= v.x;
        y /= v.y;

        return *this;
    }

    vec2_t& operator+=(float fl)
    {
        x += fl;
        y += fl;

        return *this;
    }

    vec2_t& operator-=(float fl)
    {
        x -= fl;
        y -= fl;

        return *this;
    }

    vec2_t& operator*=(float fl)
    {
        x *= fl;
        y *= fl;

        return *this;
    }

    vec2_t& operator/=(float fl)
    {
        x /= fl;
        y /= fl;

        return *this;
    }

    vec2_t operator+(const vec2_t& v) const
    {
        vec2_t result;

        result.x = x + v.x;
        result.y = y + v.y;

        return result;
    }

    vec2_t operator-(const vec2_t& v) const
    {
        vec2_t delta;

        delta.x = x - v.x;
        delta.y = y - v.y;

        return delta;
    }

    vec2_t operator*(const vec2_t& v) const
    {
        vec2_t result;

        result.x = x * v.x;
        result.y = y * v.y;

        return result;
    }

    vec2_t operator/(const vec2_t& v) const
    {
        vec2_t result;

        result.x = x / v.x;
        result.y = y / v.y;

        return result;
    }

    vec2_t operator+(float fl) const
    {
        return { x + fl, y + fl };
    }

    vec2_t operator-(float fl) const
    {
        return { x - fl, y - fl };
    }

    vec2_t operator*(float fl) const
    {
        return { x * fl, y * fl };
    }

    vec2_t operator/(float fl) const
    {
        return { x / fl, y / fl };
    }

    [[nodiscard]] float length() const
    {
        return sqrtf(powf(x, 2) + powf(y, 2));
    }

    [[nodiscard]] bool is_zero() const
    {
        return std::fpclassify(x) == FP_ZERO && std::fpclassify(y) == FP_ZERO;
    }

    void normalize()
    {
        this->x = std::isfinite(this->x) ? std::remainder(this->x, 360.f) : 0.f;
        this->y = std::isfinite(this->y) ? std::remainder(this->y, 360.f) : 0.f;
    }
    float x, y;
};

struct vec3_t
{
    vec3_t() : x {}, y {}, z {}
    {
    }

    explicit vec3_t(float t) : x { t }, y { t }, z { t }
    {
    }
    explicit vec3_t(int t) : x { static_cast<float>(t) }, y { static_cast<float>(t) }, z { static_cast<float>(t) }
    {
    }

    vec3_t(float x, float y, float z) : x { x }, y { y }, z { z }
    {
    }

    vec3_t(int x, int y, int z) : x { static_cast<float>(x) }, y { static_cast<float>(y) }, z { static_cast<float>(z) }
    {
    }

    explicit vec3_t(const float* array) : x(array[0]), y(array[1]), z(array[2])
    {
    }
    explicit vec3_t(const int* array) : x(static_cast<float>(array[0])), y(static_cast<float>(array[1])), z(static_cast<float>(array[2]))
    {
    }

    vec3_t& operator+=(const vec3_t& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    vec3_t& operator-=(const vec3_t& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    vec3_t& operator*=(const vec3_t& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;

        return *this;
    }

    vec3_t& operator/=(const vec3_t& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;

        return *this;
    }

    vec3_t& operator+=(float fl)
    {
        x += fl;
        y += fl;
        z += fl;

        return *this;
    }

    vec3_t& operator-=(float fl)
    {
        x -= fl;
        y -= fl;
        z -= fl;

        return *this;
    }

    vec3_t& operator*=(float fl)
    {
        x *= fl;
        y *= fl;
        z *= fl;

        return *this;
    }

    vec3_t& operator/=(float fl)
    {
        x /= fl;
        y /= fl;
        z /= fl;

        return *this;
    }

    vec3_t operator+(const vec3_t& v) const
    {
        vec3_t result;

        result.x = x + v.x;
        result.y = y + v.y;
        result.z = z + v.z;

        return result;
    }

    vec3_t operator-(const vec3_t& v) const
    {
        vec3_t delta;

        delta.x = x - v.x;
        delta.y = y - v.y;
        delta.z = z - v.z;

        return delta;
    }

    vec3_t operator*(const vec3_t& v) const
    {
        vec3_t result;

        result.x = x * v.x;
        result.y = y * v.y;
        result.z = z * v.z;

        return result;
    }

    vec3_t operator/(const vec3_t& v) const
    {
        vec3_t result;

        result.x = x / v.x;
        result.y = y / v.y;
        result.z = z / v.z;

        return result;
    }

    vec3_t operator+(float fl) const
    {
        return { x + fl, y + fl, z + fl };
    }

    vec3_t operator-(float fl) const
    {
        return { x - fl, y - fl, z - fl };
    }

    vec3_t operator*(float fl) const
    {
        return { x * fl, y * fl, z * fl };
    }

    vec3_t operator/(float fl) const
    {
        return { x / fl, y / fl, z / fl };
    }

    [[nodiscard]] float length_2d() const
    {
        return sqrtf(powf(x, 2) + powf(y, 2));
    }

    [[nodiscard]] float length() const
    {
        return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
    }

    [[nodiscard]] float length_2d_sqrt() const
    {
        return powf(x, 2) + powf(y, 2);
    }

    [[nodiscard]] float length_sqrt() const
    {
        return powf(x, 2) + powf(y, 2) + powf(z, 2);
    }

    float vector_normalize(vec3_t& v) const
    {
        auto radius = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
        radius      = 1.f / (radius + FLT_EPSILON);

        v *= radius;

        return radius;
    }

    [[nodiscard]] vec3_t normalized() const
    {
        auto v = *this;
        vector_normalize(v);
        return v;
    }

    [[nodiscard]] vec2_t to_2d() const
    {
        return { x, y };
    }

    [[nodiscard]] float dot(vec3_t dot) const
    {
        return x * dot.x + y * dot.y + z * dot.z;
    }

    float dot(float& dot) const
    {
        return x * dot + y * dot + z * dot;
    }

    void normalize()
    {
        x = std::isfinite(x) ? std::remainder(x, 360.f) : 0.f;
        y = std::isfinite(y) ? std::remainder(y, 360.f) : 0.f;
        z = 0.f;
    }

    [[nodiscard]] bool is_zero() const
    {
        return std::fpclassify(x) == FP_ZERO && std::fpclassify(y) == FP_ZERO && std::fpclassify(z) == FP_ZERO;
    }

    float x, y, z;
};

struct vec4_t
{
    float x, y, z, w;
    vec4_t() : x {}, y {}, z {}, w {}
    {
    }

    vec4_t(float x, float y, float z, float w) : x { x }, y { y }, z { z }, w { w }
    {
    }

    vec4_t& operator+=(const vec4_t& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;

        return *this;
    }

    vec4_t& operator-=(const vec4_t& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;

        return *this;
    }

    vec4_t& operator*=(const vec4_t& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;

        return *this;
    }

    vec4_t& operator/=(const vec4_t& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;

        return *this;
    }

    vec4_t& operator+=(float fl)
    {
        x += fl;
        y += fl;
        z += fl;
        w += fl;

        return *this;
    }

    vec4_t& operator-=(float fl)
    {
        x -= fl;
        y -= fl;
        z -= fl;
        w -= fl;

        return *this;
    }

    vec4_t& operator*=(float fl)
    {
        x *= fl;
        y *= fl;
        z *= fl;
        w *= fl;

        return *this;
    }

    vec4_t& operator/=(float fl)
    {
        x /= fl;
        y /= fl;
        z /= fl;
        w /= fl;

        return *this;
    }

    vec4_t operator+(const vec4_t& v) const
    {
        vec4_t result;

        result.x = x + v.x;
        result.y = y + v.y;
        result.z = z + v.z;
        result.w = w + v.w;

        return result;
    }

    vec4_t operator-(const vec4_t& v) const
    {
        vec4_t delta;

        delta.x = x - v.x;
        delta.y = y - v.y;
        delta.z = z - v.z;
        delta.w = w - v.w;

        return delta;
    }

    vec4_t operator*(const vec4_t& v) const
    {
        vec4_t result;

        result.x = x * v.x;
        result.y = y * v.y;
        result.z = z * v.z;
        result.w = w * v.w;

        return result;
    }

    vec4_t operator/(const vec4_t& v) const
    {
        vec4_t result;

        result.x = x / v.x;
        result.y = y / v.y;
        result.z = z / v.z;
        result.w = w / v.w;

        return result;
    }

    vec4_t operator+(float fl) const
    {
        return { x + fl, y + fl, z + fl, w + fl };
    }

    vec4_t operator-(float fl) const
    {
        return { x - fl, y - fl, z - fl, w - fl };
    }

    vec4_t operator*(float fl) const
    {
        return { x * fl, y * fl, z * fl, w * fl };
    }

    vec4_t operator/(float fl) const
    {
        return { x / fl, y / fl, z / fl, w / fl };
    }
};

class quaternion;
struct angle_t;

class radian_euler
{
public:
    inline radian_euler(void)
    {
    }

    inline radian_euler(float X, float Y, float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }

    inline radian_euler(quaternion const& q);   // evil auto type promotion!!!
    inline radian_euler(angle_t const& angles); // evil auto type promotion!!!

    // Initialization
    inline void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
    {
        x = ix;
        y = iy;
        z = iz;
    }

    //	conversion to qangle
    angle_t to_angle(void) const;

    bool is_valid() const;

    void invalidate();

    inline float* base()
    {
        return &x;
    }

    inline const float* base() const
    {
        return &x;
    }

    // array access...
    float operator[](int i) const;

    float& operator[](int i);

    float x, y, z;
};

class quaternion // same data-layout as engine's vec4_t,
{                //		which is a float[4]
public:
    inline quaternion(void)
    {
    }

    inline quaternion(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw)
    {
    }

    inline quaternion(radian_euler const& angle); // evil auto type promotion!!!

    inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f)
    {
        x = ix;
        y = iy;
        z = iz;
        w = iw;
    }

    bool is_valid() const;

    void invalidate();

    bool operator==(const quaternion& src) const;

    bool operator!=(const quaternion& src) const;

    float* base()
    {
        return (float*)this;
    }

    const float* base() const
    {
        return (float*)this;
    }

    // array access...
    float operator[](int i) const;

    float& operator[](int i);

    float x, y, z, w;
};
