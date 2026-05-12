// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: unit tests for the generic ColorControl converters
// (kFzColor decode + kTzColor encode).

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// lightingColorCtrl AttributeReport carrying:
//   attr 0x0003 currentX type 0x21 u16 = 0x4000 (~0.25)
//   attr 0x0004 currentY type 0x21 u16 = 0x8000 (0.5)
//   attr 0x0008 colorMode type 0x30 enum8 = 0x01 (xy)
constexpr std::uint8_t kColorXyFrame[] = {
    0x18, 0x42, 0x0A,                           // ZCL header
    0x03, 0x00, 0x21, 0x00, 0x40,                // 0x0003 u16 0x4000
    0x04, 0x00, 0x21, 0x00, 0x80,                // 0x0004 u16 0x8000
    0x08, 0x00, 0x30, 0x01,                      // 0x0008 enum8 1
};

// lightingColorCtrl AttributeReport with hue/saturation/colorTemp:
//   attr 0x0000 currentHue type 0x20 u8 = 120
//   attr 0x0001 currentSaturation type 0x20 u8 = 200
//   attr 0x0007 colorTemperature type 0x21 u16 = 250
constexpr std::uint8_t kColorHsFrame[] = {
    0x18, 0x43, 0x0A,
    0x00, 0x00, 0x20, 0x78,                      // 0x0000 u8 = 120
    0x01, 0x00, 0x20, 0xC8,                      // 0x0001 u8 = 200
    0x07, 0x00, 0x21, 0xFA, 0x00,                // 0x0007 u16 = 250
};

bool nearly_equal(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) < eps;
}

}  // namespace

// ── fz_color: XY decode ────────────────────────────────────────────

static void test_fz_color_decodes_xy() {
    auto raw = build_frame(0x0300, kColorXyFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "lightingColorCtrl";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));

    const Value* x = out.find("color_x");
    const Value* y = out.find("color_y");
    const Value* mode = out.find("color_mode");

    assert(x && x->type == ValueType::Float);
    assert(nearly_equal(x->f, 0x4000 / 65535.0f));
    assert(y && y->type == ValueType::Float);
    assert(nearly_equal(y->f, 0x8000 / 65535.0f));
    assert(mode && mode->type == ValueType::StringRef);
    assert(std::strcmp(mode->str, "xy") == 0);
}

// ── fz_color: hue / saturation / color_temp decode ─────────────────

static void test_fz_color_decodes_hue_sat_temp() {
    auto raw = build_frame(0x0300, kColorHsFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "lightingColorCtrl";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));

    const Value* hue = out.find("hue");
    const Value* sat = out.find("saturation");
    const Value* ct  = out.find("color_temp");

    assert(hue && hue->type == ValueType::Uint && hue->u == 120);
    assert(sat && sat->type == ValueType::Uint && sat->u == 200);
    assert(ct  && ct->type  == ValueType::Uint && ct->u  == 250);
}

// ── fz_color: descriptor ───────────────────────────────────────────

static void test_fz_color_descriptor_cluster() {
    assert(generic::kFzColor.cluster);
    assert(std::strcmp(generic::kFzColor.cluster, "lightingColorCtrl") == 0);
    assert(generic::kFzColor.direction == Direction::ServerToClient);
}

// ── tz_color: color_x / color_y → moveToColor (0x07) ──────────────

static void test_tz_color_x_emits_move_to_color() {
    std::uint8_t buf[16]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Float; v.f = 0.5f;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::tz_color("color_x", v, ::zhc::generic::kTzColor, def, ctx, buf, n));

    // moveToColor (0x07): x u16 LE, y u16 LE (centre 0x8000), transition 0.
    // 0.5 * 65535 + 0.5 = 32767.5 → 0x8000.
    const std::uint8_t want[] = {
        0x11, 0x00, 0x07,
        0x00, 0x80,        // x = 0x8000
        0x00, 0x80,        // y = 0x8000 (centre default)
        0x00, 0x00,        // transition = 0
    };
    assert(n == sizeof(want));
    assert(std::memcmp(buf, want, n) == 0);
}

// ── tz_color: hue → moveToHue (0x00) ──────────────────────────────

static void test_tz_color_hue_emits_move_to_hue() {
    std::uint8_t buf[16]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Uint; v.u = 120;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::tz_color("hue", v, ::zhc::generic::kTzColor, def, ctx, buf, n));

    // moveToHue (0x00): hue u8, direction u8 (0=shortest), transition u16 LE.
    const std::uint8_t want[] = {
        0x11, 0x00, 0x00,
        0x78,              // hue = 120
        0x00,              // direction = 0
        0x00, 0x00,        // transition = 0
    };
    assert(n == sizeof(want));
    assert(std::memcmp(buf, want, n) == 0);
}

// ── tz_color: saturation → moveToSaturation (0x03) ────────────────

static void test_tz_color_sat_emits_move_to_saturation() {
    std::uint8_t buf[16]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Uint; v.u = 200;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::tz_color("saturation", v, ::zhc::generic::kTzColor, def, ctx, buf, n));

    // moveToSaturation (0x03): sat u8, transition u16 LE.
    const std::uint8_t want[] = {
        0x11, 0x00, 0x03,
        0xC8,              // sat = 200
        0x00, 0x00,        // transition = 0
    };
    assert(n == sizeof(want));
    assert(std::memcmp(buf, want, n) == 0);
}

// ── tz_color: rejects unrelated keys ──────────────────────────────

static void test_tz_color_rejects_unknown_key() {
    std::uint8_t buf[16]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Uint; v.u = 1;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(!generic::tz_color("brightness", v, ::zhc::generic::kTzColor, def, ctx, buf, n));
    assert(n == 0);
}

// ── tz_color: descriptor ──────────────────────────────────────────

static void test_tz_color_descriptor() {
    assert(generic::kTzColor.cluster_id == 0x0300);
    assert(generic::kTzColor.key == nullptr);  // wildcard
}

int main() {
    test_fz_color_decodes_xy();
    test_fz_color_decodes_hue_sat_temp();
    test_fz_color_descriptor_cluster();
    test_tz_color_x_emits_move_to_color();
    test_tz_color_hue_emits_move_to_hue();
    test_tz_color_sat_emits_move_to_saturation();
    test_tz_color_rejects_unknown_key();
    test_tz_color_descriptor();
    return 0;
}
