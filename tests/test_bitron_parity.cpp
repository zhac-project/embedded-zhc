// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the SMaBiT (Bitron Video) AV2010 IAS-Zone sensors.
// z2m-source: bitron.ts.
//
// Bug fixed: AV2010/33 (vibration sensor) is wired in z2m to
// `fz.ias_occupancy_alarm_2`, which reads zoneStatus **bit 1** (alarm_2),
// not bit 0. The generated def lowered kFzIasVibrationAlarm, whose label
// reads bit 0 (matching the four OTHER vibration sensors that use
// `fz.ias_vibration_alarm_1`). On AV2010/33 that meant the vibration event —
// signalled on alarm_2 — never reached the shadow. The def was graduated to
// a Tier-2 parent override using the new bit-1 variant kFzIasVibrationAlarm2.
//
// These tests pin, on a real Zone-Status-Change-Notification wire shape:
//   1. AV2010/33 decodes "vibration" from bit 1 (and NOT from bit 0), with
//      the shared tamper/battery_low bits (2/3).
//   2. A representative bit-0 sensor (AV2010/22 motion) is untouched: it
//      still decodes "occupancy" from bit 0. This guards against the shared
//      kLbl_Vibration label having been mutated instead of adding a variant.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::bitron {
extern const PreparedDefinition kDef_AV2010_33;  // vibration (alarm_2 / bit 1)
extern const PreparedDefinition kDef_AV2010_22;  // occupancy (alarm_1 / bit 0)
}  // namespace zhc::devices::bitron

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

// AV2010/33: vibration must come from bit 1 (alarm_2), never bit 0.
void test_av2010_33_vibration_bit1() {
    const auto& def = devices::bitron::kDef_AV2010_33;
    assert(def_exposes(def, "vibration"));
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "occupancy"));

    // bit 1 set -> vibration true.
    auto on = dispatch_ias(def, ias_notif(0x0002));   // alarm_2 only
    assert(on.any_matched);
    assert(b_true(on.merged.find("vibration")));
    assert(on.merged.find("alarm") == nullptr);
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    // bit 0 set (alarm_1) must NOT trigger vibration — this is the bug guard.
    auto bit0 = dispatch_ias(def, ias_notif(0x0001));
    assert(bit0.any_matched);
    assert(b_false(bit0.merged.find("vibration")));

    // clear.
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("vibration")));

    // tamper(bit2)+battery_low(bit3) without alarm bits.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("vibration")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// Guard: the shared bit-0 vibration/occupancy path is untouched. AV2010/22 is
// a motion sensor (fz.ias_occupancy_alarm_1_with_timeout, bit 0).
void test_av2010_22_occupancy_bit0() {
    const auto& def = devices::bitron::kDef_AV2010_22;
    assert(def_exposes(def, "occupancy"));

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 / bit 0
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));
    assert(on.merged.find("alarm") == nullptr);

    auto bit1 = dispatch_ias(def, ias_notif(0x0002));  // bit 1 must NOT trip it
    assert(bit1.any_matched);
    assert(b_false(bit1.merged.find("occupancy")));
}

}  // namespace

int main() {
    test_av2010_33_vibration_bit1();
    test_av2010_22_occupancy_bit0();
    return 0;
}
