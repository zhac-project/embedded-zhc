// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Exercises dispatch_to_zigbee against the real generic::kTzOnOff wired
// into a real device (tuya::kDefTS0001). Keeps the encoder + selector
// plumbing honest end-to-end.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "definitions/_generic/_shared.hpp"
#include "zhc/devices/tuya_registry.hpp"

// Generated Tuya definitions are reachable through the registry table, not by
// name; declare the one this test writes to.
namespace zhc::devices::tuya { extern const PreparedDefinition kDef_THAH202001; }
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0001;
}

using namespace zhc;

static void test_ts0001_on() {
    RuntimeContext ctx{};
    std::uint8_t frame[8] = {};
    Value v{}; v.type = ValueType::Bool; v.b = true;

    auto r = dispatch_to_zigbee(devices::tuya::kDefTS0001, "state", v, ctx,
                                 frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0006);
    assert(r.frame_size == 3);
    // write_header's kCmdFc (0x11) + TSN placeholder (0x00) + cmd on (0x01).
    assert(frame[0] == 0x11);
    assert(frame[1] == 0x00);
    assert(frame[2] == 0x01);
}

static void test_ts0001_off_string() {
    RuntimeContext ctx{};
    std::uint8_t frame[8] = {};
    Value v{}; v.type = ValueType::StringRef; v.str = "OFF";

    auto r = dispatch_to_zigbee(devices::tuya::kDefTS0001, "state", v, ctx,
                                 frame);
    assert(r.ok);
    assert(frame[2] == 0x00);
}

static void test_ts0001_unknown_key() {
    RuntimeContext ctx{};
    std::uint8_t frame[8] = {};
    Value v{}; v.type = ValueType::Bool; v.b = true;

    auto r = dispatch_to_zigbee(devices::tuya::kDefTS0001, "brightness", v,
                                 ctx, frame);
    assert(!r.ok);
}

// ── tz_zcl_write_attr — generic writeAttributes encoder ────────
static void test_zcl_write_bool_non_manu() {
    using namespace zhc::generic;
    static constexpr ZclWriteSpec spec{
        .key = "child_lock",
        .attr_id = 0x8000,
        .attr_type = 0x10,
        .manufacturer_code = 0,
        .lookup = nullptr,
        .lookup_count = 0,
    };
    TzConverter cvt{
        .key = "child_lock",
        .cluster = "genOnOff",
        .cluster_id = 0x0006,
        .command_id = 0x02,
        .fn = &tz_zcl_write_attr,
        .user_config = &spec,
    };
    RuntimeContext ctx{};
    std::uint8_t frame[16] = {};
    std::size_t n = 0;
    Value v{}; v.type = ValueType::Bool; v.b = true;
    assert(tz_zcl_write_attr("child_lock", v, cvt, PreparedDefinition{},
                              ctx, frame, n));
    // fc=0x10 tsn=0x00 cmd=0x02 attr=0x8000 type=0x10 val=0x01
    const std::uint8_t want[] = {0x10, 0x00, 0x02, 0x00, 0x80, 0x10, 0x01};
    assert(n == sizeof(want));
    assert(std::memcmp(frame, want, n) == 0);
}

static void test_zcl_write_enum_manu_specific() {
    using namespace zhc::generic;
    static constexpr ZclWriteLookup lut[] = {
        {"off", 0}, {"on", 1}, {"previous", 2},
    };
    static constexpr ZclWriteSpec spec{
        .key = "power_outage_memory",
        .attr_id = 0x0201,
        .attr_type = 0x30,
        .manufacturer_code = 0x115F,
        .lookup = lut,
        .lookup_count = 3,
    };
    TzConverter cvt{
        .key = "power_outage_memory",
        .cluster = "manuSpecificLumi",
        .cluster_id = 0xFCC0,
        .command_id = 0x02,
        .fn = &tz_zcl_write_attr,
        .user_config = &spec,
    };
    RuntimeContext ctx{};
    std::uint8_t frame[16] = {};
    std::size_t n = 0;
    Value v{}; v.type = ValueType::StringRef; v.str = "previous";
    assert(tz_zcl_write_attr("power_outage_memory", v, cvt,
                              PreparedDefinition{}, ctx, frame, n));
    // fc=0x14 manu_lo=0x5F manu_hi=0x11 tsn=0x00 cmd=0x02 attr=0x0201 type=0x30 val=0x02
    const std::uint8_t want[] = {
        0x14, 0x5F, 0x11, 0x00, 0x02, 0x01, 0x02, 0x30, 0x02,
    };
    assert(n == sizeof(want));
    assert(std::memcmp(frame, want, n) == 0);
}

// A decimal on a raw attribute write is rounded, not refused or truncated:
// 21.6 -> 22 on an int16 attribute (the wire unit is whatever the attribute
// uses; the caller chose the value).
static void test_zcl_write_float_rounds() {
    using namespace zhc::generic;
    static constexpr ZclWriteSpec spec{
        .key = "level",
        .attr_id = 0x0010,
        .attr_type = 0x29,   // int16
        .manufacturer_code = 0,
        .lookup = nullptr,
        .lookup_count = 0,
    };
    TzConverter cvt{
        .key = "level", .cluster = "genBasic", .cluster_id = 0x0000, .command_id = 0x02,
        .fn = &tz_zcl_write_attr, .user_config = &spec,
    };
    RuntimeContext ctx{};
    std::uint8_t frame[16] = {};
    std::size_t n = 0;
    Value v{}; v.type = ValueType::Float; v.f = 21.6f;
    assert(tz_zcl_write_attr("level", v, cvt, PreparedDefinition{}, ctx, frame, n));
    const std::uint8_t want[] = {0x10, 0x00, 0x02, 0x10, 0x00, 0x29, 22, 0x00};
    assert(n == sizeof(want));
    assert(std::memcmp(frame, want, n) == 0);
    v.f = -0.4f;   // rounds toward zero at |x| < 0.5
    assert(tz_zcl_write_attr("level", v, cvt, PreparedDefinition{}, ctx, frame, n));
    assert(frame[6] == 0 && frame[7] == 0);
}

// A decimal on a Tuya numeric datapoint with an integer divisor scales
// BEFORE rounding: 21.5 on a divisor-10 DP is 215 on the wire, not 210.
// THAH202001 dp102 "scale_protection_remaining_time" has divisor 10.
static void test_tuya_numeric_float_scales() {
    RuntimeContext ctx{};
    std::uint8_t frame[64] = {};
    Value v{}; v.type = ValueType::Float; v.f = 21.5f;
    auto r = dispatch_to_zigbee(devices::tuya::kDef_THAH202001,
                                "scale_protection_remaining_time", v, ctx,
                                std::span<std::uint8_t>(frame, sizeof(frame)));
    assert(r.ok);
    assert(r.frame_size >= 4);
    // Numeric DP value: 4 bytes big-endian s32 at the end of the frame.
    const std::uint8_t* tail = frame + r.frame_size - 4;
    const std::int32_t got = (std::int32_t)(((std::uint32_t)tail[0] << 24) | ((std::uint32_t)tail[1] << 16) |
                                            ((std::uint32_t)tail[2] << 8) | tail[3]);
    assert(got == 215);
    // The integer path is unchanged: 21 -> 210.
    Value u{}; u.type = ValueType::Uint; u.u = 21;
    r = dispatch_to_zigbee(devices::tuya::kDef_THAH202001, "scale_protection_remaining_time", u, ctx,
                           std::span<std::uint8_t>(frame, sizeof(frame)));
    assert(r.ok);
    tail = frame + r.frame_size - 4;
    assert(tail[3] == 210 && tail[2] == 0);
}

int main() {
    test_ts0001_on();
    test_ts0001_off_string();
    test_ts0001_unknown_key();
    test_zcl_write_bool_non_manu();
    test_zcl_write_enum_manu_specific();
    test_zcl_write_float_rounds();
    test_tuya_numeric_float_scales();
    return 0;
}
