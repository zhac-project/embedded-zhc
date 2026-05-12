// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Exercises dispatch_to_zigbee against the real generic::kTzOnOff wired
// into a real device (tuya::kDefTS0001). Keeps the encoder + selector
// plumbing honest end-to-end.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "definitions/_generic/_shared.hpp"
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

int main() {
    test_ts0001_on();
    test_ts0001_off_string();
    test_ts0001_unknown_key();
    test_zcl_write_bool_non_manu();
    test_zcl_write_enum_manu_specific();
    return 0;
}
