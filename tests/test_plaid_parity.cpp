// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the PLAID SYSTEMS Spruce temperature & moisture sensor.
// z2m-source: plaid.ts #PS-SPRZMS-SLP3
//   fromZigbee: [fz.temperature, fz.humidity, fz.plaid_battery]
//   exposes:    humidity, temperature, battery, battery_voltage
//
// Bug fixed: the auto-port dropped BOTH sensor channels — temperature
// (msTemperatureMeasurement 0x0402) and humidity (msRelativeHumidity
// 0x0405) — and instead emitted a phantom on/off bundle (kFzOnOff +
// `state` expose + 0x0006 bind), leaving a moisture sensor reduced to
// battery-only. This pins the restored decoders on real ZCL wire shapes:
//   - msTemperatureMeasurement measuredValue (s16, /100) -> temperature
//   - msRelativeHumidity      measuredValue (u16, /100) -> humidity
//   - genPowerCfg mainsVoltage (attr 0x0000, raw mV)     -> voltage + battery
//     where battery = toPercentage(voltage, 2500, 3000) per z2m
//     meta.battery.voltageToPercentage. Also asserts the phantom `state`
//     expose and 0x0006 genOnOff bind are gone.

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

namespace zhc::devices::plaid {
extern const PreparedDefinition kDef_PS_SPRZMS_SLP3;
}  // namespace zhc::devices::plaid

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

bool f_near(const Value* v, float expect) {
    if (!v || v->type != ValueType::Float) return false;
    const float d = v->f - expect;
    return (d > -0.001f) && (d < 0.001f);
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

// Exposes/bindings shape: temperature + humidity + battery + voltage; the
// phantom `state` on/off expose and 0x0006 genOnOff bind are gone.
static void test_exposes_and_binds() {
    const auto& def = devices::plaid::kDef_PS_SPRZMS_SLP3;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    assert(!def_exposes(def, "state"));            // phantom on/off gone
    assert(def_binds(def, 0x0001));                // genPowerCfg
    assert(def_binds(def, 0x0402));                // msTemperatureMeasurement
    assert(def_binds(def, 0x0405));                // msRelativeHumidity
    assert(!def_binds(def, 0x0006));               // phantom genOnOff gone
}

// msTemperatureMeasurement measuredValue s16 /100. 2350 -> 23.50 °C.
static void test_temperature() {
    const auto& def = devices::plaid::kDef_PS_SPRZMS_SLP3;
    const std::uint8_t val[] = {0x2E, 0x09};   // 0x092E = 2350
    auto r = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 1,
                          attr_report(0x0000, 0x29, val));   // 0x29 = int16
    assert(r.any_matched);
    assert(f_near(r.merged.find("temperature"), 23.5f));
}

// Negative temperature: s16 -550 -> -5.50 °C.
static void test_temperature_negative() {
    const auto& def = devices::plaid::kDef_PS_SPRZMS_SLP3;
    const std::uint8_t val[] = {0xDA, 0xFD};   // 0xFDDA = -550
    auto r = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 1,
                          attr_report(0x0000, 0x29, val));
    assert(r.any_matched);
    assert(f_near(r.merged.find("temperature"), -5.5f));
}

// msRelativeHumidity measuredValue u16 /100. 5012 -> 50.12 %.
static void test_humidity() {
    const auto& def = devices::plaid::kDef_PS_SPRZMS_SLP3;
    const std::uint8_t val[] = {0x94, 0x13};   // 0x1394 = 5012
    auto r = dispatch_zcl(def, 0x0405, "msRelativeHumidity", 1,
                          attr_report(0x0000, 0x21, val));   // 0x21 = uint16
    assert(r.any_matched);
    assert(f_near(r.merged.find("humidity"), 50.12f));
}

// genPowerCfg mainsVoltage (attr 0x0000, u16 raw mV). 2750 mV ->
// voltage=2750, battery=toPercentage(2750,2500,3000)=round(50.0)=50.
static void test_battery_midpoint() {
    const auto& def = devices::plaid::kDef_PS_SPRZMS_SLP3;
    const std::uint8_t val[] = {0xBE, 0x0A};   // 0x0ABE = 2750
    auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                          attr_report(0x0000, 0x21, val));   // 0x21 = uint16
    assert(r.any_matched);
    assert(u_eq(r.merged.find("voltage"), 2750));
    assert(u_eq(r.merged.find("battery"), 50));
}

// Full battery: 3000 mV (and above) clamps to 100 %.
static void test_battery_full() {
    const auto& def = devices::plaid::kDef_PS_SPRZMS_SLP3;
    const std::uint8_t val[] = {0xB8, 0x0B};   // 0x0BB8 = 3000
    auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                          attr_report(0x0000, 0x21, val));
    assert(r.any_matched);
    assert(u_eq(r.merged.find("voltage"), 3000));
    assert(u_eq(r.merged.find("battery"), 100));
}

// Empty battery: 2400 mV clamps below min (2500) to 0 %.
static void test_battery_empty() {
    const auto& def = devices::plaid::kDef_PS_SPRZMS_SLP3;
    const std::uint8_t val[] = {0x60, 0x09};   // 0x0960 = 2400
    auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                          attr_report(0x0000, 0x21, val));
    assert(r.any_matched);
    assert(u_eq(r.merged.find("voltage"), 2400));
    assert(u_eq(r.merged.find("battery"), 0));
}

int main() {
    test_exposes_and_binds();
    test_temperature();
    test_temperature_negative();
    test_humidity();
    test_battery_midpoint();
    test_battery_full();
    test_battery_empty();
    return 0;
}
