// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the xyzroe DIY USB/DC power-monitor family
// (ZigUSB / ZigUSB_C6 / ZigDC).
//
// z2m-source: zigbee-herdsman-converters/src/devices/xyzroe.ts
//
// Channel-by-channel investigation summary:
//
//   ZigUSB (CC2530): voltage/current/power + uptime are encoded over
//     genAnalogInput (0x000C) with a vendor `description`-string parser
//     (fzLocal.zigusb_analog_input); on/off is INVERTED
//     (fzLocal.zigusb_on_off_invert). No generic 0x000C converter and no
//     inverting on/off converter exist -> those channels are INFRA and
//     stay deferred. The one standard-cluster channel is fz.temperature
//     (msTemperatureMeasurement 0x0402) which the generic kFzTemperature
//     decodes -- pinned below.
//
//   ZigDC: INA3221 voltage/current/power flow through the vendor
//     fzLocal.ZigDC_ina3221 on genAnalogInput (0x000C) -> INFRA. The
//     standard-cluster channels are fz.temperature (0x0402) +
//     fz.humidity (0x0405), decoded by kFzTemperature / kFzHumidity --
//     pinned below.
//
//   ZigUSB_C6 (ESP32-C6): built entirely from modernExtend --
//     m.electricityMeter({cluster:"electrical", electricalMeasurementType:
//     "both"}) + m.temperature() + m.onOff()x3 + m.iasZoneAlarm(
//     {zoneType:"generic", zoneAttributes:["alarm_1"]}). This is the def
//     that carried real parity gaps, fixed in this pass:
//       (a) cluster:"electrical" maps ONLY to fz.electrical_measurement
//           (haElectricalMeasurement 0x0B04) and exposes power/voltage/
//           current -- it has NO seMetering and NO `energy`. The earlier
//           port wired kFzMetering + an `energy` expose + a 0x0702 bind;
//           all three were phantom and are removed.
//       (b) the m.iasZoneAlarm over-current alarm (ssIasZone 0x0500
//           StatusChangeNotification -> alarm_1) had been dropped; it is
//           restored via the generic kFzIasZoneStatusChange.
//
// These tests pin, at the dispatch_from_zigbee boundary, the
// standard-cluster channels that the generic converters cover, plus the
// ZigUSB_C6 static shape (phantom `energy`/0x0702 gone; alarm_1/0x0500
// present).

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

namespace zhc::devices::xyzroe {
extern const PreparedDefinition kDef_ZigUSB;
extern const PreparedDefinition kDef_ZigUSB_C6;
extern const PreparedDefinition kDef_ZigDC;
}  // namespace zhc::devices::xyzroe

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

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

// ── 9-byte IAS Zone Status Change Notification (ssIasZone cmd 0x00) ───
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
DispatchResult dispatch_ias(const PreparedDefinition& def, std::uint16_t zone_status,
                            std::uint8_t src_ep) {
    const std::array<std::uint8_t, 9> bytes = {
        0x09, 0x42, 0x00,
        static_cast<std::uint8_t>(zone_status & 0xFF),
        static_cast<std::uint8_t>(zone_status >> 8),
        0x00, 0x00, 0x00, 0x00};
    auto raw = make_frame(0x0500, src_ep,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── ZigUSB_C6 — the def that carried the real parity gaps. ──────────
//
// endpoint_map labels {1,2,3} drive the dispatch suffix rewrite, so the
// generic converters surface as: power_1 / current_1 / voltage (forced
// global) / temperature_3 / state_1 / state_2 / state_3 / alarm_1_1.
void check_zigusb_c6() {
    const auto& def = zhc::devices::xyzroe::kDef_ZigUSB_C6;

    // Static shape: every runtime-emitted channel is declared.
    assert(def_exposes(def, "state_1"));
    assert(def_exposes(def, "state_2"));         // status-LED switch (was missing)
    assert(def_exposes(def, "state_3"));         // USB-state-LED switch (was missing)
    assert(def_exposes(def, "power_1"));
    assert(def_exposes(def, "voltage"));         // forced-global key
    assert(def_exposes(def, "current_1"));
    assert(def_exposes(def, "temperature_3"));
    assert(def_exposes(def, "alarm_1_1"));       // over-current alarm (restored)

    // Phantom-removal regression guards: cluster:"electrical" has NO energy.
    assert(!def_exposes(def, "energy"));
    assert(!def_binds(def, 0x0702));             // seMetering must be gone

    assert(def_binds(def, 0x0006));              // genOnOff
    assert(def_binds(def, 0x0B04));              // haElectricalMeasurement
    assert(def_binds(def, 0x0500));              // ssIasZone (restored)
    assert(def_binds(def, 0x0402));              // msTemperatureMeasurement

    // haElectricalMeasurement (0x0B04) on ep1 → power_1 / voltage / current_1.
    // attr 0x050B ActivePower int16s = 0x000A = 10 W.
    const std::uint8_t power_b[] = {0x0A, 0x00};
    auto rp = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x050B, 0x29, power_b));
    assert(rp.any_matched);
    const Value* pv = rp.merged.find("power_1");
    assert(pv && pv->type == ValueType::Int && pv->i == 10);

    // attr 0x0505 RMSVoltage uint16 = 0x1388 = 5000 (key stays bare "voltage").
    const std::uint8_t volt_b[] = {0x88, 0x13};
    auto rv = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0505, 0x21, volt_b));
    assert(rv.any_matched);
    const Value* vv = rv.merged.find("voltage");
    assert(vv && vv->type == ValueType::Uint && vv->u == 5000);

    // attr 0x0508 RMSCurrent uint16 = 0x01F4 = 500 → current_1.
    const std::uint8_t curr_b[] = {0xF4, 0x01};
    auto rc = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0508, 0x21, curr_b));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("current_1");
    assert(cv && cv->type == ValueType::Uint && cv->u == 500);

    // m.temperature() — msTemperatureMeasurement (0x0402) on ep3 → temperature_3.
    const std::uint8_t temp_b[] = {0x34, 0x08};   // 2100 raw
    auto rt = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 3,
                           attr_report(0x0000, 0x29, temp_b));
    assert(rt.any_matched);
    assert(rt.merged.find("temperature_3") != nullptr);

    // m.onOff() x3 — each switch endpoint yields its own state_<n>.
    const std::uint8_t on_b[] = {0x01};
    auto ron1 = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                             attr_report(0x0000, 0x10, on_b));
    assert(ron1.any_matched && b_true(ron1.merged.find("state_1")));
    auto ron2 = dispatch_zcl(def, 0x0006, "genOnOff", 2,
                             attr_report(0x0000, 0x10, on_b));
    assert(ron2.any_matched && b_true(ron2.merged.find("state_2")));
    auto ron3 = dispatch_zcl(def, 0x0006, "genOnOff", 3,
                             attr_report(0x0000, 0x10, on_b));
    assert(ron3.any_matched && b_true(ron3.merged.find("state_3")));

    // m.iasZoneAlarm — over-current on ep1 → alarm_1_1. bit0 set → true; clear → false.
    auto ion = dispatch_ias(def, 0x0001, 1);
    assert(ion.any_matched && b_true(ion.merged.find("alarm_1_1")));
    auto ioff = dispatch_ias(def, 0x0000, 1);
    assert(ioff.any_matched && b_false(ioff.merged.find("alarm_1_1")));
}

// ── ZigUSB — only the standard-cluster temperature channel is portable. ─
void check_zigusb() {
    const auto& def = zhc::devices::xyzroe::kDef_ZigUSB;

    // Static shape (INFRA channels still declared for the SPA).
    assert(def_exposes(def, "state_l1"));
    assert(def_exposes(def, "temperature_l4"));
    assert(def_binds(def, 0x0402));             // msTemperatureMeasurement (ep4)

    // fz.temperature on 0x0402 (ep4) fires through the generic converter.
    const std::uint8_t temp_b[] = {0x34, 0x08};   // 2100 raw
    auto rt = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 4,
                           attr_report(0x0000, 0x29, temp_b));
    assert(rt.any_matched);
    assert(rt.merged.find("temperature") != nullptr);
}

// ── ZigDC — standard-cluster temperature + humidity are portable. ──
void check_zigdc() {
    const auto& def = zhc::devices::xyzroe::kDef_ZigDC;

    assert(def_exposes(def, "temperature_l6"));
    assert(def_exposes(def, "humidity_l6"));
    assert(def_binds(def, 0x0402));             // msTemperatureMeasurement
    assert(def_binds(def, 0x0405));             // msRelativeHumidity
    // No state-set path on ZigDC.
    assert(def.to_zigbee_count == 0);

    // fz.temperature on 0x0402 (ep6).
    const std::uint8_t temp_b[] = {0x34, 0x08};   // 2100 raw
    auto rt = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 6,
                           attr_report(0x0000, 0x29, temp_b));
    assert(rt.any_matched);
    assert(rt.merged.find("temperature") != nullptr);

    // fz.humidity on 0x0405 (ep6) attr 0x0000 uint16 = 0x1388 = 5000 raw (50.00%).
    const std::uint8_t hum_b[] = {0x88, 0x13};
    auto rh = dispatch_zcl(def, 0x0405, "msRelativeHumidity", 6,
                           attr_report(0x0000, 0x21, hum_b));
    assert(rh.any_matched);
    assert(rh.merged.find("humidity") != nullptr);
}

}  // namespace

int main() {
    check_zigusb_c6();
    check_zigusb();
    check_zigdc();
    return 0;
}
