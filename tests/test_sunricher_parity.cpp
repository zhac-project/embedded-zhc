// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Sunricher device family. Pins the real gaps in the
// auto-generated ports against z2m wire shapes:
//
//   1. IAS alarm sensors — SR-ZG9070A-SS (smoke), SR-ZG9060A-GS (gas),
//      SR-ZG9060B-CS (carbon monoxide), SR-ZG9050C-WS / SR-ZG9050B-WS
//      (water leak) and SR-ZG9011A-DS (contact) wired the generic kFzIasZone,
//      which emits the bare key "alarm" behind an expose that also declared
//      "alarm". z2m's iasZoneAlarm({zoneType}) publishes a semantic key
//      (smoke / gas / carbon_monoxide / water_leak / contact) with no rename
//      layer, so a consumer keyed on z2m's key never saw the alarm. Fixed by
//      the typed kFzIas*Alarm converters + matching exposes.
//
//   2. SR-ZG9030F-PS human-presence sensor was mis-ported as a controllable
//      on/off RELAY (genOnOff, expose "state"). It has no relay — its outputs
//      are occupancy (msOccupancySensing) and illuminance. Fixed by
//      kFzOccupancy + kFzIlluminance.
//
//   3. HK-SENSOR-4IN1-A (occupancy + temp + humidity + illuminance) and
//      SR-ZG9033TH (temp + humidity) lowered only kFzBattery, dropping every
//      measurement the device exists to report. SR-ZG9032A-PIR lowered only
//      the ep1 light and dropped occupancy (ep2) + illuminance (ep3). Fixed by
//      adding the generic kFzOccupancy / kFzTemperature / kFzHumidity /
//      kFzIlluminance decoders + exposes.
//
// z2m-source: zigbee-herdsman-converters/src/devices/sunricher.ts.

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#include <string>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::sunricher {
extern const PreparedDefinition kDef_SR_ZG9070A_SS;   // smoke
extern const PreparedDefinition kDef_SR_ZG9060A_GS;   // gas
extern const PreparedDefinition kDef_SR_ZG9060B_CS;   // carbon_monoxide
extern const PreparedDefinition kDef_SR_ZG9050C_WS;   // water_leak
extern const PreparedDefinition kDef_SR_ZG9050B_WS;   // water_leak
extern const PreparedDefinition kDef_SR_ZG9011A_DS;   // contact
extern const PreparedDefinition kDef_SR_ZG9030F_PS;   // presence → occupancy + illuminance
extern const PreparedDefinition kDef_HK_SENSOR_4IN1_A; // occ + temp + humidity + lux
extern const PreparedDefinition kDef_SR_ZG9033TH;     // temp + humidity
extern const PreparedDefinition kDef_SR_ZG9032A_PIR;  // light + occupancy + illuminance
}  // namespace zhc::devices::sunricher

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

// The dispatcher fans out a bare per-property expose key to `<key>_<label>`
// at runtime when the device opts into multi-endpoint via `def.endpoint_map`
// (see src/runtime/dispatch.cpp). Mirror that here to know which shadow key a
// decode on `src_ep` lands under. Single-EP devices keep the bare key.
std::string merged_key(const PreparedDefinition& def, const char* base,
                       std::uint8_t src_ep) {
    if (def.endpoint_map && def.endpoint_map_count > 0) {
        for (std::uint8_t i = 0; i < def.endpoint_map_count; ++i) {
            if (def.endpoint_map[i].endpoint == src_ep && def.endpoint_map[i].label) {
                return std::string(base) + "_" + def.endpoint_map[i].label;
            }
        }
    }
    return std::string(base);
}

// Dispatch a decoded message on a given source endpoint, supplying the cluster
// name the wire frame implies (decode_frame leaves id→name mapping to caller).
DispatchResult dispatch_zcl_ep(const PreparedDefinition& def, std::uint16_t cluster_id,
                               std::uint8_t src_ep, const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    return dispatch_zcl_ep(def, cluster_id, 1, cluster_name, bytes);
}

// ── IAS Zone Status Change Notification (ssIasZone cmd 0x00) ──────────
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
    return dispatch_zcl(def, 0x0500, "ssIasZone",
                        std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
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

// ── Typed IAS alarm: semantic key set on bit 0, bare "alarm" must be gone ─
void check_ias_alarm(const PreparedDefinition& def, const char* sem_key) {
    // Regression guard: semantic key exposed, dead "alarm" gone.
    assert(def_exposes(def, sem_key));
    assert(!def_exposes(def, "alarm"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));

    auto fired = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 (bit 0)
    assert(fired.any_matched);
    assert(b_true(fired.merged.find(sem_key)));
    assert(fired.merged.find("alarm") == nullptr);       // dead key must be gone

    auto clear = dispatch_ias(def, ias_notif(0x0000));
    assert(clear.any_matched);
    assert(b_false(clear.merged.find(sem_key)));

    // Tamper (bit 2) + battery_low (bit 3) still pass through the typed cvt.
    auto tb = dispatch_ias(def, ias_notif(0x000C));      // bits 2 and 3
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── Occupancy: msOccupancySensing attr 0x0000 bitmap8, bit 0 → bool ───
void check_occupancy(const PreparedDefinition& def, std::uint8_t src_ep) {
    assert(def_exposes(def, "occupancy"));
    const std::string key = merged_key(def, "occupancy", src_ep);
    const std::uint8_t occ[] = {0x01};
    auto r = dispatch_zcl_ep(def, 0x0406, src_ep, "msOccupancySensing",
                             attr_report(0x0000, 0x18, occ));
    assert(r.any_matched);
    assert(b_true(r.merged.find(key.c_str())));

    const std::uint8_t clr[] = {0x00};
    auto rc = dispatch_zcl_ep(def, 0x0406, src_ep, "msOccupancySensing",
                              attr_report(0x0000, 0x18, clr));
    assert(b_false(rc.merged.find(key.c_str())));
}

// ── Illuminance: msIlluminanceMeasurement attr 0x0000 u16, raw (z2m identity) ─
void check_illuminance(const PreparedDefinition& def, std::uint8_t src_ep) {
    assert(def_exposes(def, "illuminance"));
    const std::uint8_t lux[] = {0xE8, 0x03};            // 0x03E8 = 1000
    auto r = dispatch_zcl_ep(def, 0x0400, src_ep, "msIlluminanceMeasurement",
                             attr_report(0x0000, 0x21, lux));
    assert(r.any_matched);
    const Value* lv = r.merged.find(merged_key(def, "illuminance", src_ep).c_str());
    assert(lv && lv->type == ValueType::Uint && lv->u == 1000);
}

// ── Temperature: msTemperatureMeasurement attr 0x0000 s16 (×0.01) ─────
void check_temperature(const PreparedDefinition& def, std::uint8_t src_ep) {
    assert(def_exposes(def, "temperature"));
    const std::uint8_t t[] = {0x09, 0x09};               // 0x0909 = 2313 → 23.13 °C
    auto r = dispatch_zcl_ep(def, 0x0402, src_ep, "msTemperatureMeasurement",
                             attr_report(0x0000, 0x29, t));
    assert(r.any_matched);
    const Value* tv = r.merged.find(merged_key(def, "temperature", src_ep).c_str());
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(tv->f - 23.13f) < 0.01f);
}

// ── Humidity: msRelativeHumidity attr 0x0000 u16 (×0.01) ──────────────
void check_humidity(const PreparedDefinition& def, std::uint8_t src_ep) {
    assert(def_exposes(def, "humidity"));
    const std::uint8_t h[] = {0xE8, 0x13};               // 0x13E8 = 5096 → 50.96 %
    auto r = dispatch_zcl_ep(def, 0x0405, src_ep, "msRelativeHumidity",
                             attr_report(0x0000, 0x21, h));
    assert(r.any_matched);
    const Value* hv = r.merged.find(merged_key(def, "humidity", src_ep).c_str());
    assert(hv && hv->type == ValueType::Float);
    assert(std::fabs(hv->f - 50.96f) < 0.01f);
}

}  // namespace

int main() {
    using namespace zhc::devices::sunricher;

    // 1. IAS dead-key fixes.
    check_ias_alarm(kDef_SR_ZG9070A_SS, "smoke");
    check_ias_alarm(kDef_SR_ZG9060A_GS, "gas");
    check_ias_alarm(kDef_SR_ZG9060B_CS, "carbon_monoxide");
    check_ias_alarm(kDef_SR_ZG9050C_WS, "water_leak");
    check_ias_alarm(kDef_SR_ZG9050B_WS, "water_leak");
    check_ias_alarm(kDef_SR_ZG9011A_DS, "contact");

    // 2. Presence sensor: occupancy + illuminance, relay "state" gone.
    assert(!def_exposes(kDef_SR_ZG9030F_PS, "state"));
    check_occupancy(kDef_SR_ZG9030F_PS, 1);
    check_illuminance(kDef_SR_ZG9030F_PS, 1);

    // 3a. 4-in-1: occupancy + temperature + humidity + illuminance (all ep1).
    check_occupancy(kDef_HK_SENSOR_4IN1_A, 1);
    check_temperature(kDef_HK_SENSOR_4IN1_A, 1);
    check_humidity(kDef_HK_SENSOR_4IN1_A, 1);
    check_illuminance(kDef_HK_SENSOR_4IN1_A, 1);

    // 3b. TH sensor: temperature (ep1) + humidity (ep2).
    check_temperature(kDef_SR_ZG9033TH, 1);
    check_humidity(kDef_SR_ZG9033TH, 2);

    // 3c. PIR/light hybrid: light retained, occupancy (ep2) + illuminance (ep3) added.
    assert(def_exposes(kDef_SR_ZG9032A_PIR, "state"));
    assert(def_exposes(kDef_SR_ZG9032A_PIR, "brightness"));
    check_occupancy(kDef_SR_ZG9032A_PIR, 2);
    check_illuminance(kDef_SR_ZG9032A_PIR, 3);

    return 0;
}
