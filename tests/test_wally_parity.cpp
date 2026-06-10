// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Wally WallyHome multi-sensor (U02I007C.01,
// zigbeeModel "MultiSensor"). z2m-source: wally.ts.
//
// Bugs fixed (found by a z2m<->embedded-zhc parity pass):
//   * The generated def degraded the device to kFzOnOff + kTzOnOff + dead
//     `state`, dropped the temperature + humidity channels entirely, and
//     lowered the generic kFzIasZone (bare `alarm`) behind a dead `alarm`
//     expose. z2m instead:
//       - decodes fz.temperature / fz.humidity,
//       - treats genOnOff On/Off as command actions (fz.command_on/off →
//         action "on"/"off"), with NO toZigbee,
//       - splits the IAS into two PER-ENDPOINT converters over ssIasZone
//         status_change: endpoint 1 → contact = !(bit0), endpoint 2 →
//         water_leak = (bit0).
//
// These tests pin, on real ZCL wire shapes, that each semantic channel
// decodes its key, that the per-endpoint IAS routing does NOT cross-fire,
// and that the dead `state` / `alarm` keys are gone.

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

namespace zhc::devices::wally {
extern const PreparedDefinition kDef_U02I007C_01;
}  // namespace zhc::devices::wally

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool str_is(const Value* v, const char* want) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01 (C->S), tsn, cmd, payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd) {
    return {0x01, 0x42, cmd};
}

// ssIasZone commandStatusChangeNotification (cmd 0x00): fc=0x09 (S->C bit
// set), tsn, cmd, zoneStatus u16 LE, extStatus u8, zoneId u8, delay u16.
std::vector<std::uint8_t> ias_status(std::uint16_t zone_status) {
    return {0x09, 0x42, 0x00,
            static_cast<std::uint8_t>(zone_status & 0xFF),
            static_cast<std::uint8_t>(zone_status >> 8),
            0x00, 0x00, 0x00, 0x00};
}

// ZCL attribute report (fc=0x18, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

}  // namespace

// ── exposes / binds shape ────────────────────────────────────────────
static void test_shape() {
    const auto& def = zhc::devices::wally::kDef_U02I007C_01;
    // Real channels present.
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "contact"));
    assert(def_exposes(def, "water_leak"));
    // Phantom relay + dead bare-alarm keys removed.
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "alarm"));
    // z2m has no toZigbee — device is not a sink.
    assert(def.to_zigbee_count == 0);
    // Binds: power/onoff/temp/humidity on ep1; IAS zone on BOTH endpoints.
    assert(def_binds(def, 1, 0x0001));
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0402));
    assert(def_binds(def, 1, 0x0405));
    assert(def_binds(def, 1, 0x0500));
    assert(def_binds(def, 2, 0x0500));
}

// ── temperature + humidity ───────────────────────────────────────────
static void test_temperature_humidity() {
    const auto& def = zhc::devices::wally::kDef_U02I007C_01;
    // temperature: msTemperatureMeasurement 0x0402 attr 0x0000 s16 (2292 = 22.92 C)
    const std::uint8_t t[] = {0xF4, 0x08};
    auto rt = dispatch_zcl(def, 0x0402, 1, "msTemperatureMeasurement",
                           attr_report(0x0000, 0x29, t));
    assert(rt.any_matched);
    assert(rt.merged.find("temperature") != nullptr);
    // humidity: msRelativeHumidity 0x0405 attr 0x0000 u16 (5000 = 50.00 %)
    const std::uint8_t h[] = {0x88, 0x13};
    auto rh = dispatch_zcl(def, 0x0405, 1, "msRelativeHumidity",
                           attr_report(0x0000, 0x21, h));
    assert(rh.any_matched);
    assert(rh.merged.find("humidity") != nullptr);
}

// ── command_on / command_off → action ───────────────────────────────
static void test_command_action() {
    const auto& def = zhc::devices::wally::kDef_U02I007C_01;
    // genOnOff On (cmd 0x01) → action "on".
    auto on = dispatch_zcl(def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched);
    assert(str_is(on.merged.find("action"), "on"));
    // No phantom controllable `state` is emitted.
    assert(on.merged.find("state") == nullptr);
    // genOnOff Off (cmd 0x00) → action "off".
    auto off = dispatch_zcl(def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
    assert(off.any_matched);
    assert(str_is(off.merged.find("action"), "off"));
}

// ── per-endpoint IAS routing (no cross-fire) ─────────────────────────
static void test_ias_endpoint_routing() {
    const auto& def = zhc::devices::wally::kDef_U02I007C_01;

    // endpoint 1, bit0 SET → contact = false (open), water_leak NOT emitted.
    auto ep1_on = dispatch_zcl(def, 0x0500, 1, "ssIasZone", ias_status(0x0001));
    assert(ep1_on.any_matched);
    assert(b_false(ep1_on.merged.find("contact")));      // !bit0
    assert(ep1_on.merged.find("water_leak") == nullptr); // ep2 converter gated out
    assert(ep1_on.merged.find("alarm") == nullptr);

    // endpoint 1, bit0 CLEAR → contact = true (closed).
    auto ep1_off = dispatch_zcl(def, 0x0500, 1, "ssIasZone", ias_status(0x0000));
    assert(ep1_off.any_matched);
    assert(b_true(ep1_off.merged.find("contact")));
    assert(ep1_off.merged.find("water_leak") == nullptr);

    // endpoint 2, bit0 SET → water_leak = true, contact NOT emitted.
    auto ep2_on = dispatch_zcl(def, 0x0500, 2, "ssIasZone", ias_status(0x0001));
    assert(ep2_on.any_matched);
    assert(b_true(ep2_on.merged.find("water_leak")));
    assert(ep2_on.merged.find("contact") == nullptr);    // ep1 converter gated out
    assert(ep2_on.merged.find("alarm") == nullptr);

    // endpoint 2, bit0 CLEAR → water_leak = false.
    auto ep2_off = dispatch_zcl(def, 0x0500, 2, "ssIasZone", ias_status(0x0000));
    assert(ep2_off.any_matched);
    assert(b_false(ep2_off.merged.find("water_leak")));
    assert(ep2_off.merged.find("contact") == nullptr);
}

int main() {
    test_shape();
    test_temperature_humidity();
    test_command_action();
    test_ias_endpoint_routing();
    return 0;
}
