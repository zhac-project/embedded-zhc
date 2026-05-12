// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: unit + integration tests for the generic vendor-neutral
// converters (fz_on_off, fz_battery, fz_ignore).

#include <cassert>
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

// genOnOff attr 0x0000 bool, value 0x01 (on).
constexpr std::uint8_t kOnFrame[] = {
    0x18, 0x01, 0x0A,           // ZCL header
    0x00, 0x00, 0x10, 0x01,      // attr 0x0000, type 0x10 bool, value on
};

// genOnOff attr 0x0000 bool, value 0x00 (off).
constexpr std::uint8_t kOffFrame[] = {
    0x18, 0x02, 0x0A,
    0x00, 0x00, 0x10, 0x00,
};

// genPowerCfg: two records — attr 0x0020 (voltage, u8=29), attr 0x0021
// (battery %, u8=168).
constexpr std::uint8_t kBatteryFrame[] = {
    0x18, 0x03, 0x0A,
    0x20, 0x00, 0x20, 0x1D,      // attr 0x0020 type 0x20 u8 = 29
    0x21, 0x00, 0x20, 0xA8,      // attr 0x0021 type 0x20 u8 = 168
};

// msOccupancySensing attr 0x0000 bitmap8=0x01.
constexpr std::uint8_t kOccupancyFrame[] = {
    0x18, 0x04, 0x0A,
    0x00, 0x00, 0x18, 0x01,
};

}  // namespace

// ── fz_on_off ───────────────────────────────────────────────────────

static void test_on_off_decodes_on() {
    auto raw = build_frame(0x0006, kOnFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::fz_on_off(msg, generic::kFzOnOff, def, ctx, out));

    const Value* state = out.find("state");
    assert(state && state->type == ValueType::Bool && state->b == true);
}

static void test_on_off_decodes_off() {
    auto raw = build_frame(0x0006, kOffFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::fz_on_off(msg, generic::kFzOnOff, def, ctx, out));

    const Value* state = out.find("state");
    assert(state && state->type == ValueType::Bool && state->b == false);
}

// ── fz_battery ──────────────────────────────────────────────────────

static void test_battery_emits_raw_voltage_and_percent() {
    auto raw = build_frame(0x0001, kBatteryFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genPowerCfg";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::fz_battery(msg, generic::kFzBattery, def, ctx, out));

    const Value* v = out.find("voltage");
    assert(v && v->type == ValueType::Uint && v->u == 29);

    const Value* b = out.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 168);
}

// ── fz_ignore ───────────────────────────────────────────────────────

static void test_ignore_matches_but_emits_nothing() {
    auto raw = build_frame(0x0406, kOccupancyFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msOccupancySensing";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::fz_ignore(msg, generic::kFzIgnoreOccupancyReport,
                               def, ctx, out));
    assert(out.count == 0);
}

// ── Adapter integration: a generic on/off device with ONLY
// kFzOnOff in its from_zigbee list routes cleanly through dispatch
// without pulling in any lumi-specific code.
static void test_dispatch_generic_on_off_device() {
    // Compose a minimal PreparedDefinition in place. Lives on the stack
    // for the test — production definitions sit in rodata as usual.
    const FzConverter* const fz_list[] = { &generic::kFzOnOff };
    const char* const models[] = { "GENERIC_LIGHT" };

    PreparedDefinition def{};
    def.zigbee_models       = models;
    def.zigbee_models_count = 1;
    def.model               = "GENERIC_LIGHT";
    def.vendor              = "Generic";
    def.from_zigbee         = fz_list;
    def.from_zigbee_count   = 1;

    auto raw = build_frame(0x0006, kOnFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);

    const Value* state = result.merged.find("state");
    assert(state && state->type == ValueType::Bool && state->b == true);
}

// ── Outbound encoders ───────────────────────────────────────────────
//
// Byte-exact parity against z2m's `toZigbee.on_off` / `light_brightness`
// / `light_colortemp`. TSN sits at offset [1] and is always 0 — the
// platform adapter patches it before TX so tests pin the full frame.

static void test_tz_on_off_on() {
    std::uint8_t buf[8]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::StringRef; v.str = "ON";
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::tz_on_off("state", v, ::zhc::generic::kTzOnOff, def, ctx, buf, n));

    const std::uint8_t want[] = { 0x11, 0x00, 0x01 };
    assert(n == sizeof(want));
    assert(std::memcmp(buf, want, n) == 0);
}

static void test_tz_on_off_off_bool() {
    std::uint8_t buf[8]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Bool; v.b = false;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::tz_on_off("state", v, ::zhc::generic::kTzOnOff, def, ctx, buf, n));

    const std::uint8_t want[] = { 0x11, 0x00, 0x00 };
    assert(n == sizeof(want));
    assert(std::memcmp(buf, want, n) == 0);
}

static void test_tz_on_off_toggle() {
    std::uint8_t buf[8]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::StringRef; v.str = "TOGGLE";
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::tz_on_off("state", v, ::zhc::generic::kTzOnOff, def, ctx, buf, n));

    const std::uint8_t want[] = { 0x11, 0x00, 0x02 };
    assert(n == sizeof(want));
    assert(std::memcmp(buf, want, n) == 0);
}

static void test_tz_on_off_wrong_key_rejected() {
    std::uint8_t buf[8]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(!generic::tz_on_off("brightness", v, ::zhc::generic::kTzOnOff, def, ctx, buf, n));
    assert(n == 0);
}

static void test_tz_brightness_mid_level() {
    std::uint8_t buf[8]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Uint; v.u = 128;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::tz_brightness("brightness", v, ::zhc::generic::kTzBrightness, def, ctx, buf, n));

    // moveToLevelWithOnOff (0x04) : level=128, transition=0
    const std::uint8_t want[] = { 0x11, 0x00, 0x04, 0x80, 0x00, 0x00 };
    assert(n == sizeof(want));
    assert(std::memcmp(buf, want, n) == 0);
}

static void test_tz_brightness_out_of_range_rejected() {
    std::uint8_t buf[8]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Uint; v.u = 300;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(!generic::tz_brightness("brightness", v, ::zhc::generic::kTzBrightness, def, ctx, buf, n));
    assert(n == 0);
}

static void test_tz_color_temp_250_mireds() {
    std::uint8_t buf[8]{};
    std::size_t  n{};
    Value v{}; v.type = ValueType::Uint; v.u = 250;
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::tz_color_temp("color_temp", v, ::zhc::generic::kTzColorTemp, def, ctx, buf, n));

    // moveToColorTemp (0x0A) : colortemp=250 LE, transition=0 LE
    const std::uint8_t want[] = { 0x11, 0x00, 0x0A, 0xFA, 0x00, 0x00, 0x00 };
    assert(n == sizeof(want));
    assert(std::memcmp(buf, want, n) == 0);
}

static void test_tz_descriptor_cluster_ids() {
    assert(generic::kTzOnOff.cluster_id      == 0x0006);
    assert(generic::kTzBrightness.cluster_id == 0x0008);
    assert(generic::kTzColorTemp.cluster_id  == 0x0300);
    assert(std::strcmp(generic::kTzOnOff.key,      "state")      == 0);
    assert(std::strcmp(generic::kTzBrightness.key, "brightness") == 0);
    assert(std::strcmp(generic::kTzColorTemp.key,  "color_temp") == 0);
}

int main() {
    test_on_off_decodes_on();
    test_on_off_decodes_off();
    test_battery_emits_raw_voltage_and_percent();
    test_ignore_matches_but_emits_nothing();
    test_dispatch_generic_on_off_device();

    test_tz_on_off_on();
    test_tz_on_off_off_bool();
    test_tz_on_off_toggle();
    test_tz_on_off_wrong_key_rejected();
    test_tz_brightness_mid_level();
    test_tz_brightness_out_of_range_rejected();
    test_tz_color_temp_250_mireds();
    test_tz_descriptor_cluster_ids();
    return 0;
}
