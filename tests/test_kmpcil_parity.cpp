// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the KMPCIL multi-sensor family.
// z2m-source: kmpcil.ts
//   #KMPCIL-tag-001 (zigbeeModel "tagv1") — arrival sensor
//   #KMPCIL_RES005  (zigbeeModel "RES005") — environment sensor
//
// Bug fixed: KMPCIL-tag-001 declares e.battery() and z2m's presence_power
// converter applies meta.battery.voltageToPercentage "3V_1500_2800" to
// publish `battery`, but the ported kFzKmpcilPresencePower emitted only
// `voltage` — the battery expose was dead. The shared converter now also
// emits battery % via the non-linear 3V_1500_2800 formula
// (round(clamp(235 - 370000/(voltage_mV+1), 0, 100))).
//
// These tests pin, on real ZCL attribute-report wire shapes:
//   - tag-001 genPowerCfg batteryVoltage -> voltage(mV) AND battery(%)
//   - tag-001 genBinaryInput presentValue bitfield -> power_state /
//     vibration / occupancy / presence (regression guard, unchanged)
//   - RES005 genBinaryInput/genBinaryOutput presentValue -> occupancy /
//     state (regression guard for the bespoke decoders)

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::kmpcil {
extern const PreparedDefinition kDef_KMPCIL_tag_001;   // arrival sensor
extern const PreparedDefinition kDef_KMPCIL_RES005;    // environment sensor
}  // namespace zhc::devices::kmpcil

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool u_eq(const Value* v, std::uint64_t expect) {
    return v && v->type == ValueType::Uint && v->u == expect;
}

bool b_eq(const Value* v, bool expect) {
    return v && v->type == ValueType::Bool && v->b == expect;
}

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// genPowerCfg (0x0001) attr 0x0020 batteryVoltage u8 (units 100 mV).
// batteryVoltage=25 -> voltage=2500 mV; battery = round(clamp(
//   235 - 370000/(2500+1), 0, 100)) = round(87.06) = 87.
static void test_tag001_battery_from_voltage() {
    const auto& def = devices::kmpcil::kDef_KMPCIL_tag_001;
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    assert(def_binds(def, 0x0001));
    const std::uint8_t val[] = {25};   // 25 * 100 = 2500 mV
    auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                          attr_report(0x0020, 0x20, val));
    assert(r.any_matched);
    assert(u_eq(r.merged.find("voltage"), 2500));
    assert(u_eq(r.merged.find("battery"), 87));
    assert(b_eq(r.merged.find("presence"), true));
}

// A lower voltage maps to a lower percentage (non-linear curve).
// batteryVoltage=20 -> 2000 mV; battery = round(235 - 370000/2001) =
// round(50.09) = 50.
static void test_tag001_battery_curve_low() {
    const auto& def = devices::kmpcil::kDef_KMPCIL_tag_001;
    const std::uint8_t val[] = {20};   // 2000 mV
    auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                          attr_report(0x0020, 0x20, val));
    assert(r.any_matched);
    assert(u_eq(r.merged.find("voltage"), 2000));
    assert(u_eq(r.merged.find("battery"), 50));
}

// genBinaryInput (0x000F) presentValue bitfield (regression guard):
//   bit0 power_state, bit1 vibration, bit2 occupancy; presence always true.
// presentValue=0x05 -> power_state=1, vibration=0, occupancy=1.
static void test_tag001_presence_bitfield() {
    const auto& def = devices::kmpcil::kDef_KMPCIL_tag_001;
    assert(def_exposes(def, "presence"));
    assert(def_exposes(def, "power_state"));
    assert(def_exposes(def, "vibration"));
    assert(def_exposes(def, "occupancy"));
    assert(def_binds(def, 0x000F));
    const std::uint8_t val[] = {0x05, 0x00, 0x00, 0x00};   // u32 presentValue
    auto r = dispatch_zcl(def, 0x000F, "genBinaryInput", 1,
                          attr_report(0x0055, 0x23, val));   // 0x23 = uint32
    assert(r.any_matched);
    assert(b_eq(r.merged.find("presence"), true));
    assert(b_eq(r.merged.find("power_state"), true));
    assert(b_eq(r.merged.find("vibration"), false));
    assert(b_eq(r.merged.find("occupancy"), true));
}

// RES005 genBinaryInput presentValue==1 -> occupancy=true (regression).
static void test_res005_occupancy() {
    const auto& def = devices::kmpcil::kDef_KMPCIL_RES005;
    assert(def_exposes(def, "occupancy"));
    const std::uint8_t val[] = {0x01};
    auto r = dispatch_zcl(def, 0x000F, "genBinaryInput", 8,
                          attr_report(0x0055, 0x10, val));   // 0x10 = boolean
    assert(r.any_matched);
    assert(b_eq(r.merged.find("occupancy"), true));
}

// RES005 genBinaryOutput presentValue!=0 -> state=true (regression).
static void test_res005_state_on() {
    const auto& def = devices::kmpcil::kDef_KMPCIL_RES005;
    assert(def_exposes(def, "state"));
    const std::uint8_t val[] = {0x01};
    auto r = dispatch_zcl(def, 0x0010, "genBinaryOutput", 8,
                          attr_report(0x0055, 0x10, val));   // 0x10 = boolean
    assert(r.any_matched);
    assert(b_eq(r.merged.find("state"), true));
}

int main() {
    test_tag001_battery_from_voltage();
    test_tag001_battery_curve_low();
    test_tag001_presence_bitfield();
    test_res005_occupancy();
    test_res005_state_on();
    return 0;
}
