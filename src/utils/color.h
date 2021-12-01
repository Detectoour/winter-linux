#pragma once

struct color_t
{
    uint8_t r, g, b, a;

    color_t() : r {}, g {}, b {}, a { 255 }
    {
    }

    color_t(uint8_t r, uint8_t g, uint8_t b) : r { r }, g { g }, b { b }, a { 255 }
    {
    }
    color_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r { r }, g { g }, b { b }, a { a }
    {
    }
    color_t(int r, int g, int b) : r { static_cast<uint8_t>(r) }, g { static_cast<uint8_t>(g) }, b { static_cast<uint8_t>(b) }, a { 255 }
    {
    }

    color_t(int r, int g, int b, int a) : r { static_cast<uint8_t>(r) }, g { static_cast<uint8_t>(g) }, b { static_cast<uint8_t>(b) }, a { static_cast<uint8_t>(a) }
    {
    }

    color_t(int r, int g, int b, float a) : r { static_cast<uint8_t>(r) }, g { static_cast<uint8_t>(g) }, b { static_cast<uint8_t>(b) }, a { static_cast<uint8_t>(a) }
    {
    }

    color_t(float r, float g, float b, float a) : r { static_cast<uint8_t>(r) }, g { static_cast<uint8_t>(g) }, b { static_cast<uint8_t>(b) }, a { static_cast<uint8_t>(a) }
    {
    }

    color_t(float r, float g, float b) : r { static_cast<uint8_t>(r) }, g { static_cast<uint8_t>(g) }, b { static_cast<uint8_t>(b) }, a { 255 }
    {
    }

    explicit color_t(const int* arr) : r { static_cast<uint8_t>(arr[0]) }, g { static_cast<uint8_t>(arr[1]) }, b { static_cast<uint8_t>(arr[2]) }, a { static_cast<uint8_t>(arr[3]) }
    {
    }

    explicit color_t(const float* arr)
     : r { static_cast<uint8_t>(arr[0] * 255.f) }, g { static_cast<uint8_t>(arr[1] * 255.f) }, b { static_cast<uint8_t>(arr[2] * 255.f) }, a { static_cast<uint8_t>(arr[3] * 255.f) }
    {
    }

    explicit color_t(const uint8_t* arr) : r { arr[0] }, g { arr[1] }, b { arr[2] }, a { arr[3] }
    {
    }

    // Both can be used for any source engine game that supports html char
    // color. For example: CS:S, TF2
    /*[[nodiscard]] std::string to_hex() const
    {
        std::stringstream ss;
        ss << "\x7";
        ss << std::hex << ( static_cast<int>( r ) << 16 |
    static_cast<int>( g ) << 8 | static_cast<int>( b ) ); return ss.str();
    }

    [[nodiscard]] std::string to_hex_alpha() const
    {
        std::stringstream ss;
        ss << "\x8";
        ss << std::hex << ( static_cast<int>( r ) << 16 |
    static_cast<int>( g ) << 8 | static_cast<int>( b ) ); ss << std::hex
    << static_cast<int>( a ); return ss.str();
    }*/
};