// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the MSH / MySmartHouse DIY ESP sensor family.
// z2m-source: msh.ts
//   #msh.AirQMon    — PTVO air-quality sensor
//   #msh.bme280psm  — outdoor thermometer w/ BME280
//   #msh.ds18b20psm — outdoor thermometer w/ DS18B20
//
// Bugs fixed (all silently dropped by the generated Tier-1 ports):
//   * msh.bme280psm: z2m extend [battery, temperature, humidity, pressure]
//     was reduced to battery-only. Wired generic kFzTemperature /
//     kFzHumidity / kFzPressure (msTemperatureMeasurement 0x0402 /100,
//     msRelativeHumidity 0x0405 /100, msPressureMeasurement 0x0403).
//   * msh.ds18b20psm: z2m extend [battery, temperature] reduced to
//     battery-only. Wired generic kFzTemperature.
//   * msh.AirQMon: mis-ported as a bare on/off switch + phantom battery.
//     z2m exposes co2 (fz.co2 on msCO2 0x040D, ppm = floor(value*1e6)) and
//     temperature (fz.temperature on 0x0402 /100, endpoint 3 / "l3"). Wired
//     generic kFzCO2 + kFzTemperature with endpoint_map {l3->3} so the
//     temperature key suffixes to `temperature_l3`. The PTVO UART/analog
//     channels (genAnalogInput) remain deferred INFRA.
//
// These tests pin, on real attribute-report wire shapes, that each restored
// channel decodes to the z2m-equivalent value and that the AirQMon phantom
// on/off / battery state is gone.

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::msh {
extern const PreparedDefinition kDef_msh_bme280psm;
extern const PreparedDefinition kDef_msh_ds18b20psm;
extern const PreparedDefinition kDef_msh_AirQMon;
}  // namespace zhc::devices::msh

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t endpoint,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = endpoint;
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

bool near(float a, float b) { return std::fabs(a - b) < 0.01f; }

bool f_eq(const Value* v, float expect) {
    return v && v->type == ValueType::Float && near(v->f, expect);
}
bool u_eq(const Value* v, std::uint64_t expect) {
    return v && v->type == ValueType::Uint && v->u == expect;
}
bool i_eq(const Value* v, std::int64_t expect) {
    return v && v->type == ValueType::Int && v->i == expect;
}

// signed-int16 measured value at attr 0x0000 (temperature).
DispatchResult report_s16(const PreparedDefinition& def, std::uint16_t cluster,
                          std::uint8_t endpoint, std::int16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,
        0x29,                  // type 0x29 = int16
        static_cast<std::uint8_t>(static_cast<std::uint16_t>(value) & 0xFF),
        static_cast<std::uint8_t>((static_cast<std::uint16_t>(value) >> 8) & 0xFF)};
    auto raw = make_frame(cluster, endpoint,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// uint16 measured value at attr 0x0000 (humidity / pressure).
DispatchResult report_u16(const PreparedDefinition& def, std::uint16_t cluster,
                          std::uint8_t endpoint, std::uint16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,
        0x21,                  // type 0x21 = uint16
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF)};
    auto raw = make_frame(cluster, endpoint,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// genPowerCfg (0x0001) batteryPercentageRemaining (attr 0x0021, u8, 2x %).
DispatchResult report_battery_pct(const PreparedDefinition& def, std::uint8_t raw_pct) {
    const std::array<std::uint8_t, 7> rep = {
        0x18, 0x01, 0x0A,
        0x21, 0x00,            // attr 0x0021 batteryPercentageRemaining
        0x20,                  // type 0x20 = uint8
        raw_pct};
    auto raw = make_frame(0x0001, 1,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0x0001);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// msCO2 measuredValue is a single-precision float (type 0x39); z2m does
// floor(measuredValue * 1e6) -> ppm.
DispatchResult report_co2_float(const PreparedDefinition& def,
                                std::uint8_t endpoint, float frac) {
    std::array<std::uint8_t, 10> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,
        0x39,                  // type 0x39 = single float
        0, 0, 0, 0};
    std::uint8_t bytes[4];
    static_assert(sizeof(float) == 4, "float must be 4 bytes");
    std::memcpy(bytes, &frac, 4);
    rep[6] = bytes[0]; rep[7] = bytes[1]; rep[8] = bytes[2]; rep[9] = bytes[3];
    auto raw = make_frame(0x040D /* msCO2 */, endpoint,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0x040D);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ===== msh.bme280psm: temperature / humidity / pressure restored =========
static void test_bme280_exposes_bindings() {
    const auto& def = devices::msh::kDef_msh_bme280psm;
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "pressure"));
    assert(def_binds(def, 0x0402));   // msTemperatureMeasurement
    assert(def_binds(def, 0x0405));   // msRelativeHumidity
    assert(def_binds(def, 0x0403));   // msPressureMeasurement
}

static void test_bme280_temperature() {
    const auto& def = devices::msh::kDef_msh_bme280psm;
    // 2173 / 100 = 21.73 °C
    auto r = report_s16(def, 0x0402, 1, 2173);
    assert(r.any_matched);
    assert(f_eq(r.merged.find("temperature"), 21.73f));
}

static void test_bme280_humidity() {
    const auto& def = devices::msh::kDef_msh_bme280psm;
    // 5512 / 100 = 55.12 %
    auto r = report_u16(def, 0x0405, 1, 5512);
    assert(r.any_matched);
    assert(f_eq(r.merged.find("humidity"), 55.12f));
}

static void test_bme280_pressure() {
    const auto& def = devices::msh::kDef_msh_bme280psm;
    // raw 1013 (kPa scaling applied on display; decoder emits raw int)
    auto r = report_u16(def, 0x0403, 1, 1013);
    assert(r.any_matched);
    assert(i_eq(r.merged.find("pressure"), 1013));
}

// ===== msh.ds18b20psm: temperature restored ==============================
static void test_ds18b20_exposes_bindings() {
    const auto& def = devices::msh::kDef_msh_ds18b20psm;
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x0402));
}

static void test_ds18b20_temperature_subzero() {
    const auto& def = devices::msh::kDef_msh_ds18b20psm;
    // -1250 / 100 = -12.50 °C (outdoor thermometer goes sub-zero)
    auto r = report_s16(def, 0x0402, 1, -1250);
    assert(r.any_matched);
    assert(f_eq(r.merged.find("temperature"), -12.50f));
}

static void test_ds18b20_battery() {
    const auto& def = devices::msh::kDef_msh_ds18b20psm;
    // batteryPercentageRemaining is 2x % per ZCL → 180/2 = 90 %
    auto r = report_battery_pct(def, 180);
    assert(r.any_matched);
    assert(u_eq(r.merged.find("battery"), 90));
}

// ===== msh.AirQMon: wrong class fixed (co2 + temperature@l3) =============
static void test_airqmon_no_phantom_onoff_battery() {
    const auto& def = devices::msh::kDef_msh_AirQMon;
    // The phantom on/off + battery the Tier-1 port invented must be gone.
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));
    assert(!def_binds(def, 0x0006));   // genOnOff
    assert(!def_binds(def, 0x0001));   // genPowerCfg
    // Real channels present.
    assert(def_exposes(def, "co2"));
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x040D));    // msCO2
    assert(def_binds(def, 0x0402));    // msTemperatureMeasurement
}

static void test_airqmon_co2() {
    const auto& def = devices::msh::kDef_msh_AirQMon;
    // 0.000800 * 1e6 = 800 ppm; co2 is on endpoint 1 → stays bare `co2`.
    auto r = report_co2_float(def, 1, 0.0008f);
    assert(r.any_matched);
    assert(u_eq(r.merged.find("co2"), 800));
    // Not also a temperature report.
    assert(r.merged.find("temperature") == nullptr);
}

static void test_airqmon_temperature_suffixed_l3() {
    const auto& def = devices::msh::kDef_msh_AirQMon;
    // temperature reported on endpoint 3 → suffixed `temperature_l3`
    // (z2m withEndpoint("l3")). 2350 / 100 = 23.50 °C.
    auto r = report_s16(def, 0x0402, 3, 2350);
    assert(r.any_matched);
    assert(f_eq(r.merged.find("temperature_l3"), 23.50f));
    // Bare `temperature` must NOT be published for the mapped endpoint.
    assert(r.merged.find("temperature") == nullptr);
}

int main() {
    test_bme280_exposes_bindings();
    test_bme280_temperature();
    test_bme280_humidity();
    test_bme280_pressure();
    test_ds18b20_exposes_bindings();
    test_ds18b20_temperature_subzero();
    test_ds18b20_battery();
    test_airqmon_no_phantom_onoff_battery();
    test_airqmon_co2();
    test_airqmon_temperature_suffixed_l3();
    return 0;
}
