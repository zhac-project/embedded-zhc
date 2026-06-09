// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Adeo (ENKI LEXMAN / LexMan) sensor + key-fob family.
// z2m-source: adeo.ts.
//
// Bugs fixed (all found by a z2m<->embedded-zhc parity pass):
//   * LDSENK06 (indoor siren), LDSENK10 (PIR), 83633206 (water leak),
//     ZB-DoorSensor-D0007 (contact), LDSENK08 (door+vibration) and 83633205
//     (4-in-1) all lowered the generic kFzIasZone, which emits alarm_1/alarm_2
//     (or the bare `alarm`) — none of which matched the declared semantic
//     expose. With no rename layer the primary sensor state never reached the
//     shadow. Each now wires the typed kFzIas<Type>Alarm (or kFzIasGenericAlarm
//     for the zoneType:"alarm" siren) emitting the semantic key directly.
//   * 83633205 additionally dropped illuminance / temperature / humidity — the
//     three measurement channels of the "4-in-1" — which are now decoded by the
//     generic standard-cluster converters.
//   * LDSENK09 (security key fob) was mis-ported as an on/off relay
//     (kFzOnOff + kTzOnOff + `state`). It is an ssIasAce sender; it now wires
//     kFzIasAceArm + kFzIasAcePanic and exposes `action`.
//
// These tests pin, on real ZCL wire shapes, that each device decodes its
// semantic key (and NOT the bare "alarm"/"alarm_1" key), and that the key fob
// decodes `action` over ssIasAce.

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

namespace zhc::devices::adeo {
extern const PreparedDefinition kDef_LDSENK06;             // siren  → generic alarm
extern const PreparedDefinition kDef_LDSENK10;             // PIR    → occupancy
extern const PreparedDefinition kDef_D83633206;            // leak   → water_leak
extern const PreparedDefinition kDef_ZB_DoorSensor_D0007;  // contact
extern const PreparedDefinition kDef_LDSENK08;             // contact (+deferred vibration)
extern const PreparedDefinition kDef_D83633205;            // 4-in-1 multi-sensor
extern const PreparedDefinition kDef_LDSENK09;             // key fob → ACE arm/panic
}  // namespace zhc::devices::adeo

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

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

// ── IAS Zone Status Change Notification (cluster 0x0500) ─────────────
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

// Assert: alarm_1 (bit 0) asserted -> semantic key true, "alarm" not used as a
// dead key for the semantic devices, tamper/battery_low reflect bits 2/3.
void check_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── ZCL attribute report builder (cluster-specific=false, Report=0x0A) ─
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
                            const char* cluster_name, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── ssIasAce command frames (cluster 0x0501) ─────────────────────────
// commandArm (0x00): fc=0x01 (cluster-specific, client->server), tsn, cmd;
//   body: arm_mode(1), code_len(1), code[...], zone_id(1).
std::vector<std::uint8_t> ace_arm(std::uint8_t arm_mode, std::uint8_t zone = 0) {
    std::vector<std::uint8_t> v{0x01, 0x42, 0x00};
    v.push_back(arm_mode);
    v.push_back(0x00);  // code_len = 0
    v.push_back(zone);
    return v;
}
// commandPanic (0x04): header only, no body.
std::vector<std::uint8_t> ace_panic() { return std::vector<std::uint8_t>{0x01, 0x42, 0x04}; }

DispatchResult dispatch_ace(const PreparedDefinition& def, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(0x0501, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasAce";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool str_is(const Value* v, const char* want) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}

}  // namespace

// ── typed IAS alarm_1 (bit 0) decoders ───────────────────────────────
static void test_siren_alarm()  { check_alarm1(devices::adeo::kDef_LDSENK06,            "alarm"); }
static void test_pir()          { check_alarm1(devices::adeo::kDef_LDSENK10,            "occupancy"); }
static void test_water_leak()   { check_alarm1(devices::adeo::kDef_D83633206,           "water_leak"); }
static void test_contact_door() { check_alarm1(devices::adeo::kDef_ZB_DoorSensor_D0007, "contact"); }
static void test_contact_vib()  { check_alarm1(devices::adeo::kDef_LDSENK08,            "contact"); }

// Regression: the bare alarm_1/alarm_2 keys produced by the old kFzIasZone must
// no longer leak for the semantic sensors (siren keeps `alarm` by design).
static void test_no_dead_alarm1_keys() {
    auto r = dispatch_ias(devices::adeo::kDef_LDSENK10, ias_notif(0x0001));
    assert(r.merged.find("alarm_1") == nullptr);
    assert(r.merged.find("alarm") == nullptr);   // PIR uses `occupancy`, not `alarm`
    auto w = dispatch_ias(devices::adeo::kDef_D83633206, ias_notif(0x0001));
    assert(w.merged.find("alarm_1") == nullptr);
    assert(w.merged.find("alarm") == nullptr);
}

// ── 83633205 — 4-in-1: occupancy via IAS + temp/humidity/illuminance ──
static void test_4in1_occupancy_via_ias() {
    const auto& def = devices::adeo::kDef_D83633205;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "illuminance"));

    // occupancy rides ssIasZone (0x0500), bit 0 — NOT msOccupancySensing.
    assert(def_binds(def, 0x0500));
    auto occ = dispatch_ias(def, ias_notif(0x0001));
    assert(occ.any_matched);
    assert(b_true(occ.merged.find("occupancy")));
    assert(occ.merged.find("alarm") == nullptr);
}
static void test_4in1_measurements() {
    const auto& def = devices::adeo::kDef_D83633205;
    // temperature: msTemperatureMeasurement 0x0402 attr 0x0000 s16 (2292 = 22.92 C)
    const std::uint8_t t[] = {0xF4, 0x08};
    auto rt = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", attr_report(0x0000, 0x29, t));
    assert(rt.any_matched);
    assert(rt.merged.find("temperature") != nullptr);
    // humidity: msRelativeHumidity 0x0405 attr 0x0000 u16 (5000 = 50.00 %)
    const std::uint8_t h[] = {0x88, 0x13};
    auto rh = dispatch_zcl(def, 0x0405, "msRelativeHumidity", attr_report(0x0000, 0x21, h));
    assert(rh.any_matched);
    assert(rh.merged.find("humidity") != nullptr);
    // illuminance: msIlluminanceMeasurement 0x0400 attr 0x0000 u16 raw
    const std::uint8_t l[] = {0x64, 0x00};
    auto rl = dispatch_zcl(def, 0x0400, "msIlluminanceMeasurement", attr_report(0x0000, 0x21, l));
    assert(rl.any_matched);
    assert(rl.merged.find("illuminance") != nullptr);
}

// ── LDSENK09 — key fob: ssIasAce arm / panic → `action` (not `state`) ─
static void test_keyfob_action() {
    const auto& def = devices::adeo::kDef_LDSENK09;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));   // phantom relay removed
    assert(def_binds(def, 0x0501));       // ssIasAce, not genOnOff
    assert(!def_binds(def, 0x0006));
    assert(def.to_zigbee_count == 0);     // not a sink

    // commandArm arm_mode 0 -> disarm.
    auto disarm = dispatch_ace(def, ace_arm(0x00));
    assert(disarm.any_matched);
    assert(str_is(disarm.merged.find("action"), "disarm"));

    // commandArm arm_mode 3 -> arm_all_zones.
    auto arm = dispatch_ace(def, ace_arm(0x03));
    assert(arm.any_matched);
    assert(str_is(arm.merged.find("action"), "arm_all_zones"));

    // commandPanic -> panic.
    auto panic = dispatch_ace(def, ace_panic());
    assert(panic.any_matched);
    assert(str_is(panic.merged.find("action"), "panic"));
}

int main() {
    test_siren_alarm();
    test_pir();
    test_water_leak();
    test_contact_door();
    test_contact_vib();
    test_no_dead_alarm1_keys();
    test_4in1_occupancy_via_ias();
    test_4in1_measurements();
    test_keyfob_action();
    return 0;
}
