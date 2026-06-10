// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Frient sensor/meter family.
// Frient is the Develco rebrand (same silicon), so these mirror the
// develco fixes.
// z2m-source: zigbee-herdsman-converters/src/devices/frient.ts.
//
// Bugs fixed (each device graduated generated/ -> Tier-2 parent):
//
//  * IAS dead-key + lost decoder: WISZB-131 lowered the generic kFzIasZone
//    converter (bare key "alarm") and dropped the msTemperatureMeasurement
//    channel. z2m wires m.iasZoneAlarm({zoneType:"contact",
//    zoneAttributes:["alarm_1","battery_low"]}) + m.temperature(): the
//    semantic key is "contact" and temperature lives on 0x0402. Per z2m the
//    contact key is INVERTED (contact = !bit0): bit0 set → contact:false
//    (open), bit0 clear → contact:true (closed). Now uses the typed
//    kFzIasContactAlarm + kFzTemperature.
//
//  * Lost 0x0B04 electrical half: SMRZB-153 (m.electricityMeter() default
//    "both") and EMIZB-151 (m.electricityMeter({threePhase,producedEnergy,
//    tariffs}), also default "both") wired only kFzMetering (0x0702), so
//    voltage/current (haElectricalMeasurement 0x0B04) were dead. Added
//    kFzElectricalMeasurement + the voltage/current exposes + 0x0B04 bind.
//
// These tests pin the corrected wire-shape decode for each fix.

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

namespace zhc::devices::frient {
extern const PreparedDefinition kDef_WISZB_131;   // contact + temperature
extern const PreparedDefinition kDef_SMRZB_153;   // onOff + metering + electrical
extern const PreparedDefinition kDef_EMIZB_151;   // metering + electrical
}  // namespace zhc::devices::frient

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool f_near(const Value* v, float expect) {
    if (!v || v->type != ValueType::Float) return false;
    const float d = v->f - expect;
    return (d < 0.01f) && (d > -0.01f);
}

// haElectricalMeasurement RMS values are emitted raw (Uint) by the generic
// converter — divisor scaling happens device-side from the cluster's
// acVoltageDivisor/acCurrentDivisor attrs, mirroring z2m's runtime scale.
bool u_eq(const Value* v, std::uint64_t expect) {
    return v && v->type == ValueType::Uint && v->u == expect;
}

// ── 9-byte IAS Zone Status Change Notification (ssIasZone cmd 0x00) ───
struct IasFrame { std::array<std::uint8_t, 9> bytes; };
IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = {0x09, tsn, 0x00,
               static_cast<std::uint8_t>(zone_status & 0xFF),
               static_cast<std::uint8_t>(zone_status >> 8),
               0x00, 0x00, 0x00, 0x00};
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

// ── WISZB-131 — typed contact key + temperature channel ──────────────
static void test_wiszb131_contact() {
    const auto& def = devices::frient::kDef_WISZB_131;
    assert(def_exposes(def, "contact"));        // semantic key present
    assert(!def_exposes(def, "alarm"));          // bare key must be gone

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 bit 0
    assert(on.any_matched);
    assert(b_false(on.merged.find("contact")));   // z2m: contact = !bit0 → open
    assert(on.merged.find("alarm") == nullptr);

    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_true(off.merged.find("contact")));   // z2m: contact = !bit0 → closed

    auto bl = dispatch_ias(def, ias_notif(0x0008));   // battery_low bit 3
    assert(bl.any_matched);
    assert(b_true(bl.merged.find("battery_low")));
}

static void test_wiszb131_temp() {
    const auto& def = devices::frient::kDef_WISZB_131;
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x0402));
    const std::uint8_t val[] = {0xF4, 0x08};   // 0x08F4 = 2292 -> 22.92 °C
    auto r = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 1,
                          attr_report(0x0000, 0x29, val));
    assert(r.any_matched);
    assert(f_near(r.merged.find("temperature"), 22.92f));
}

// ── SMRZB-153 — 0x0B04 electrical half restored ──────────────────────
static void test_smrzb153_electrical() {
    const auto& def = devices::frient::kDef_SMRZB_153;
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_binds(def, 0x0B04));
    assert(def_binds(def, 0x0702));
    // z2m endpoint:()=>({default:2}): onOff + metering live on EP2, so
    // outbound onOff must route there (else control is dead).
    assert(def.default_endpoint == 2);

    // haElectricalMeasurement rmsVoltage (0x0505) u16 -> voltage (raw Uint).
    const std::uint8_t volt[] = {0xE6, 0x00};   // 230
    auto rv = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0505, 0x21, volt));
    assert(rv.any_matched);
    assert(u_eq(rv.merged.find("voltage"), 230));

    // haElectricalMeasurement rmsCurrent (0x0508) u16 -> current (raw Uint).
    const std::uint8_t curr[] = {0xD2, 0x04};   // 1234
    auto rc = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0508, 0x21, curr));
    assert(rc.any_matched);
    assert(u_eq(rc.merged.find("current"), 1234));
}

// ── SMRZB-153 — metering (0x0702) energy still decodes ───────────────
static void test_smrzb153_metering() {
    const auto& def = devices::frient::kDef_SMRZB_153;
    // seMetering currentSummDelivered (0x0000) u48 -> energy.
    const std::uint8_t en[] = {0x10, 0x27, 0x00, 0x00, 0x00, 0x00};   // 10000
    auto r = dispatch_zcl(def, 0x0702, "seMetering", 1,
                          attr_report(0x0000, 0x25, en));
    assert(r.any_matched);
    assert(r.merged.find("energy") != nullptr);
}

// ── EMIZB-151 — 0x0B04 electrical half restored ──────────────────────
static void test_emizb151_electrical() {
    const auto& def = devices::frient::kDef_EMIZB_151;
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_exposes(def, "energy"));
    assert(def_binds(def, 0x0B04));
    assert(def_binds(def, 0x0702));

    const std::uint8_t volt[] = {0xE6, 0x00};   // 230
    auto rv = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0505, 0x21, volt));
    assert(rv.any_matched);
    assert(u_eq(rv.merged.find("voltage"), 230));
}

int main() {
    test_wiszb131_contact();
    test_wiszb131_temp();
    test_smrzb153_electrical();
    test_smrzb153_metering();
    test_emizb151_electrical();
    return 0;
}
