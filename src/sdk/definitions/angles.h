#pragma once

#include <cfloat>
#include <algorithm>

struct angle_t
{
    angle_t() : x {}, y {}, z {}
    {
    }

    angle_t(float x, float y, float z) : x { x }, y { y }, z { z }
    {
    }

    angle_t& operator+=(const angle_t& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    angle_t& operator-=(const angle_t& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    angle_t& operator*=(const angle_t& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;

        return *this;
    }

    angle_t& operator/=(const angle_t& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;

        return *this;
    }

    angle_t& operator+=(float fl)
    {
        x += fl;
        y += fl;
        z += fl;

        return *this;
    }

    angle_t& operator-=(float fl)
    {
        x -= fl;
        y -= fl;
        z -= fl;

        return *this;
    }

    angle_t& operator*=(float fl)
    {
        x *= fl;
        y *= fl;
        z *= fl;

        return *this;
    }

    angle_t& operator/=(float fl)
    {
        x /= fl;
        y /= fl;
        z /= fl;

        return *this;
    }

    angle_t operator+(const angle_t& v) const
    {
        angle_t result;

        result.x = x + v.x;
        result.y = y + v.y;
        result.z = z + v.z;

        return result;
    }

    angle_t operator-(const angle_t& v) const
    {
        angle_t delta;

        delta.x = x - v.x;
        delta.y = y - v.y;
        delta.z = z - v.z;

        return delta;
    }

    angle_t operator*(const angle_t& v) const
    {
        angle_t result;

        result.x = x * v.x;
        result.y = y * v.y;
        result.z = z * v.z;

        return result;
    }

    angle_t operator/(const angle_t& v) const
    {
        angle_t result;

        result.x = x / v.x;
        result.y = y / v.y;
        result.z = z / v.z;

        return result;
    }

    angle_t operator+(float fl) const
    {
        return { x + fl, y + fl, z + fl };
    }

    angle_t operator-(float fl) const
    {
        return { x - fl, y - fl, z - fl };
    }

    angle_t operator*(float fl) const
    {
        return { x * fl, y * fl, z * fl };
    }

    angle_t operator/(float fl) const
    {
        return { x / fl, y / fl, z / fl };
    }

    [[nodiscard]] float length_2d() const
    {
        return sqrtf(powf(x, 2) + powf(y, 2));
    }

    [[nodiscard]] float length_2d_sqr() const
    {
        return powf(x, 2) + powf(y, 2);
    }

    [[nodiscard]] float length() const
    {
        return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
    }

    [[nodiscard]] float length_sqr() const
    {
        return powf(x, 2) + powf(y, 2) + powf(z, 2);
    }

    void normalize()
    {
        this->x = std::isfinite(this->x) ? std::remainder(this->x, 360.f) : 0.f;
        this->y = std::isfinite(this->y) ? std::remainder(this->y, 360.f) : 0.f;
        this->x = std::clamp(this->x, -89.f, 89.f);
        this->y = std::clamp(this->y, -180.f, 180.f);
        this->z = 0.f;
    }

    [[nodiscard]] bool is_valid() const
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }

    float x, y, z;
};