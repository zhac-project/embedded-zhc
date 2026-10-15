// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Colour maths for `color_hex` ("#rrggbb": the light's colour at full
// brightness). Header-only, no allocation.
//  - xy <-> sRGB: CIE 1931 at Y = 1, sRGB (D65) matrix, negative channels
//    clamped, the brightest channel scaled to full (in linear light), then the
//    sRGB curve. z2m and Home Assistant use the Philips Hue wide-gamut matrix
//    instead; do not "fix" this toward them.
//  - hue (degrees) / saturation (0..1) <-> sRGB at full value (HSV).
#pragma once

#include <cmath>
#include <cstdint>

namespace zhc::color {

struct Rgb { std::uint8_t r{0}, g{0}, b{0}; };

namespace detail {
inline int hex_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
inline float to_srgb(float c) {   // linear -> sRGB
    return c <= 0.0031308f ? 12.92f * c : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
}
inline float to_linear(float c) { // sRGB -> linear
    return c <= 0.04045f ? c / 12.92f : std::pow((c + 0.055f) / 1.055f, 2.4f);
}
inline std::uint8_t to_byte(float c) {
    if (!(c > 0.0f)) return 0;
    if (c >= 1.0f) return 255;
    return static_cast<std::uint8_t>(c * 255.0f + 0.5f);
}
inline float max3(float a, float b, float c) { return a > b ? (a > c ? a : c) : (b > c ? b : c); }
inline float min3(float a, float b, float c) { return a < b ? (a < c ? a : c) : (b < c ? b : c); }
}  // namespace detail

// "#rrggbb" in either case, exactly seven characters; anything else is refused.
// `out` is left untouched when false.
// "#000000" parses: refusing black (no colour) is the caller's job.
inline bool parse_hex(const char* s, Rgb& out) {
    if (!s || s[0] != '#') return false;
    int v[6];
    for (int i = 0; i < 6; i++) {          // stops at the first non-digit, NUL included
        v[i] = detail::hex_digit(s[i + 1]);
        if (v[i] < 0) return false;
    }
    if (s[7] != '\0') return false;
    out.r = static_cast<std::uint8_t>(v[0] * 16 + v[1]);
    out.g = static_cast<std::uint8_t>(v[2] * 16 + v[3]);
    out.b = static_cast<std::uint8_t>(v[4] * 16 + v[5]);
    return true;
}

// Lower-case "#rrggbb" and its terminating NUL.
inline void format_hex(const Rgb& c, char (&out)[8]) {
    static constexpr char kDigits[] = "0123456789abcdef";
    const std::uint8_t v[3] = {c.r, c.g, c.b};
    out[0] = '#';
    for (int i = 0; i < 3; i++) {
        out[1 + 2 * i] = kDigits[v[i] >> 4];
        out[2 + 2 * i] = kDigits[v[i] & 0x0F];
    }
    out[7] = '\0';
}

// CIE xy -> sRGB. False when (x, y) is no colour: x or y not above 0, x + y
// above 1, not a number, or y so small that x / y overflows. `out` is left
// untouched when false.
inline bool xy_to_rgb(float x, float y, Rgb& out) {
    if (!(x > 0.0f) || !(y > 0.0f) || !(x + y <= 1.0f)) return false;
    const float X = x / y, Y = 1.0f, Z = (1.0f - x - y) / y;
    float r =  3.2404542f * X - 1.5371385f * Y - 0.4985314f * Z;
    float g = -0.9692660f * X + 1.8760108f * Y + 0.0415560f * Z;
    float b =  0.0556434f * X - 0.2040259f * Y + 1.0572252f * Z;
    if (r < 0.0f) r = 0.0f;
    if (g < 0.0f) g = 0.0f;
    if (b < 0.0f) b = 0.0f;
    const float m = detail::max3(r, g, b);
    if (!(m > 0.0f) || !std::isfinite(m)) return false;
    out.r = detail::to_byte(detail::to_srgb(r / m));
    out.g = detail::to_byte(detail::to_srgb(g / m));
    out.b = detail::to_byte(detail::to_srgb(b / m));
    return true;
}

// sRGB -> CIE xy. False for black, which has no chromaticity; `x` and `y` are
// left untouched then.
inline bool rgb_to_xy(const Rgb& c, float& x, float& y) {
    const float r = detail::to_linear(c.r / 255.0f);
    const float g = detail::to_linear(c.g / 255.0f);
    const float b = detail::to_linear(c.b / 255.0f);
    const float X = 0.4124564f * r + 0.3575761f * g + 0.1804375f * b;
    const float Y = 0.2126729f * r + 0.7151522f * g + 0.0721750f * b;
    const float Z = 0.0193339f * r + 0.1191920f * g + 0.9503041f * b;
    const float sum = X + Y + Z;
    if (!(sum > 0.0f)) return false;
    x = X / sum;
    y = Y / sum;
    return true;
}

// Hue in degrees (any value, wrapped), saturation 0..1 (clamped), full value.
inline Rgb hs_to_rgb(float hue_deg, float sat01) {
    float s = sat01 > 0.0f ? (sat01 < 1.0f ? sat01 : 1.0f) : 0.0f;
    float h = std::fmod(hue_deg, 360.0f);
    if (h < 0.0f) h += 360.0f;
    if (!(h >= 0.0f)) h = 0.0f;   // NaN
    auto f = [&](float n) {
        const float k = std::fmod(n + h / 60.0f, 6.0f);
        float t = k < 4.0f - k ? k : 4.0f - k;
        if (t > 1.0f) t = 1.0f;
        if (t < 0.0f) t = 0.0f;
        return 1.0f - s * t;
    };
    return Rgb{detail::to_byte(f(5.0f)), detail::to_byte(f(3.0f)), detail::to_byte(f(1.0f))};
}

// sRGB -> hue in degrees [0, 360) and saturation 0..1 (HSV; value dropped).
inline void rgb_to_hs(const Rgb& c, float& hue_deg, float& sat01) {
    const float r = c.r / 255.0f, g = c.g / 255.0f, b = c.b / 255.0f;
    const float mx = detail::max3(r, g, b), mn = detail::min3(r, g, b), d = mx - mn;
    float h = 0.0f;
    if (d > 0.0f) {
        if (mx == r)      h = std::fmod((g - b) / d, 6.0f);
        else if (mx == g) h = (b - r) / d + 2.0f;
        else              h = (r - g) / d + 4.0f;
        h *= 60.0f;
        if (h < 0.0f) h += 360.0f;
    }
    hue_deg = h;
    sat01 = mx > 0.0f ? d / mx : 0.0f;
}

}  // namespace zhc::color
