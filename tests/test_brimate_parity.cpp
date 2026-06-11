// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Brimate FZB8708HD-S1 smart motion sensor.
// z2m-source: brimate.ts (fromZigbee: [fz.ias_occupancy_alarm_1]).
//
// Bug fixed: the generated def lowered the generic kFzIasZone, which emits a
// bare `alarm` key, and declared an `alarm` expose. z2m's
// fz.ias_occupancy_alarm_1 publishes `occupancy` (zoneStatus bit 0), `tamper`
// (bit 2) and `battery_low` (bit 3), and exposes e.occupancy() +
// e.battery_low(). The def was graduated to a Tier-2 parent override using the
// typed kFzIasMotionAlarm (kLbl_Motion = "occupancy", bit 0) and the dead
// `alarm` expose was renamed to `occupancy`.
//
// These tests pin, on a real Zone-Status-Change-Notification wire shape:
//   1. The def exposes `occupancy` (and not the dead `alarm` key).
//   2. occupancy decodes from bit 0 (alarm_1), never from bit 1.
//   3. tamper (bit 2) and battery_low (bit 3) ride alongside.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::brimate {
extern const PreparedDefinition kDef_FZB8708HD_S1;  // motion (alarm_1 / bit 0)
}  // namespace zhc::devices::brimate

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// 9-byte IAS Zone Status Change Notification:
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = { 0x09, tsn, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// The expose is the semantic key, not the dead generic `alarm`.
void test_brimate_exposes_occupancy() {
    const auto& def = devices::brimate::kDef_FZB8708HD_S1;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "battery_low"));
    assert(!def_exposes(def, "alarm"));
}

// occupancy decodes from bit 0 (alarm_1), with shared tamper/battery_low bits.
void test_brimate_occupancy_bit0() {
    const auto& def = devices::brimate::kDef_FZB8708HD_S1;

    // bit 0 set -> occupancy true, no bare `alarm` key.
    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 / bit 0
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));
    assert(on.merged.find("alarm") == nullptr);
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    // bit 1 (alarm_2) must NOT trip occupancy — this is the bit guard.
    auto bit1 = dispatch_ias(def, ias_notif(0x0002));
    assert(bit1.any_matched);
    assert(b_false(bit1.merged.find("occupancy")));

    // clear.
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("occupancy")));

    // tamper(bit2)+battery_low(bit3) without the alarm bit.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("occupancy")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

int main() {
    test_brimate_exposes_occupancy();
    test_brimate_occupancy_bit0();
    return 0;
}
