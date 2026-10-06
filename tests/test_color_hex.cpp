// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// include/zhc/color_hex.hpp: hex parse/format, xy <-> sRGB, hue/sat <-> sRGB.
// Links zhc_core only, so it builds in seconds (no definitions compiled).
#include "zhc/color_hex.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using zhc::color::Rgb;

static bool near(const Rgb& c, int r, int g, int b) {
    return std::abs(c.r - r) <= 2 && std::abs(c.g - g) <= 2 && std::abs(c.b - b) <= 2;
}

int main() {
    Rgb c{};
    assert(zhc::color::parse_hex("#ff8a3d", c) && c.r == 0xff && c.g == 0x8a && c.b == 0x3d);
    assert(zhc::color::parse_hex("#FF8A3D", c) && c.g == 0x8a);
    assert(!zhc::color::parse_hex("ff8a3d", c));
    assert(!zhc::color::parse_hex("#ff8a3", c));
    assert(!zhc::color::parse_hex("#ff8a3dd", c));
    assert(!zhc::color::parse_hex("#ff8g3d", c));
    assert(!zhc::color::parse_hex("", c));
    assert(!zhc::color::parse_hex(nullptr, c));
    char out[8];
    zhc::color::format_hex(Rgb{0xff, 0x8a, 0x3d}, out);
    assert(std::strcmp(out, "#ff8a3d") == 0);

    // xy -> sRGB: the vectors zhac-tiles' ColourTest uses, +-2 per channel.
    assert(zhc::color::xy_to_rgb(0.64f, 0.33f, c) && near(c, 255, 0, 0));
    assert(zhc::color::xy_to_rgb(0.30f, 0.60f, c) && near(c, 0, 255, 0));
    assert(zhc::color::xy_to_rgb(0.15f, 0.06f, c) && near(c, 0, 0, 255));
    assert(zhc::color::xy_to_rgb(0.3127f, 0.3290f, c) && near(c, 255, 255, 255));
    assert(zhc::color::xy_to_rgb(0.4573f, 0.41f, c) && near(c, 255, 174, 91));
    assert(zhc::color::xy_to_rgb(0.2f, 0.3f, c) && near(c, 0, 233, 255));
    assert(!zhc::color::xy_to_rgb(0.3f, 0.0f, c));
    assert(!zhc::color::xy_to_rgb(-0.1f, 0.3f, c));
    assert(!zhc::color::xy_to_rgb(0.7f, 0.5f, c));
    assert(!zhc::color::xy_to_rgb(std::nanf(""), 0.3f, c));

    // sRGB -> xy, and the round trip back.
    float x = 0, y = 0;
    assert(zhc::color::rgb_to_xy(Rgb{255, 0, 0}, x, y) && std::fabs(x - 0.64f) < 0.002f && std::fabs(y - 0.33f) < 0.002f);
    assert(zhc::color::rgb_to_xy(Rgb{255, 255, 255}, x, y) && std::fabs(x - 0.3127f) < 0.002f && std::fabs(y - 0.3290f) < 0.002f);
    assert(!zhc::color::rgb_to_xy(Rgb{0, 0, 0}, x, y));
    assert(zhc::color::rgb_to_xy(Rgb{0xff, 0x8a, 0x3d}, x, y) && zhc::color::xy_to_rgb(x, y, c) && near(c, 0xff, 0x8a, 0x3d));

    // hue / saturation at full value.
    assert(near(zhc::color::hs_to_rgb(0.0f, 1.0f), 255, 0, 0));
    assert(near(zhc::color::hs_to_rgb(120.0f, 1.0f), 0, 255, 0));
    assert(near(zhc::color::hs_to_rgb(240.0f, 1.0f), 0, 0, 255));
    assert(near(zhc::color::hs_to_rgb(360.0f, 1.0f), 255, 0, 0));
    assert(near(zhc::color::hs_to_rgb(0.0f, 0.0f), 255, 255, 255));
    float h = 0, s = 0;
    zhc::color::rgb_to_hs(Rgb{0xff, 0x95, 0x00}, h, s);
    assert(std::fabs(h - 35.0f) < 0.5f && std::fabs(s - 1.0f) < 0.01f);
    zhc::color::rgb_to_hs(Rgb{0x80, 0x80, 0x80}, h, s);
    assert(s == 0.0f);

    // Branches the vectors above do not reach: g- and b-dominant hue, the hue wrap
    // below 0, sub-full saturation (the Green, Blue and Pink presets), the
    // saturation clamp, NaN hue, a seven-character string with no '#', and x == 0.
    zhc::color::rgb_to_hs(Rgb{0x34, 0xc7, 0x59}, h, s);
    assert(std::fabs(h - 135.1f) < 0.5f && std::fabs(s - 0.739f) < 0.01f);
    zhc::color::rgb_to_hs(Rgb{0x0a, 0x84, 0xff}, h, s);
    assert(std::fabs(h - 210.1f) < 0.5f && std::fabs(s - 0.961f) < 0.01f);
    zhc::color::rgb_to_hs(Rgb{0xff, 0x2d, 0x55}, h, s);
    assert(std::fabs(h - 348.6f) < 0.5f && std::fabs(s - 0.824f) < 0.01f);
    assert(near(zhc::color::hs_to_rgb(-30.0f, 1.0f), 255, 0, 128));
    assert(near(zhc::color::hs_to_rgb(30.0f, 7.0f), 255, 128, 0));
    assert(near(zhc::color::hs_to_rgb(std::nanf(""), 1.0f), 255, 0, 0));
    assert(!zhc::color::parse_hex("0ff8a3d", c));
    assert(!zhc::color::xy_to_rgb(0.0f, 0.3f, c));
    std::puts("color_hex: all passed");
    return 0;
}
