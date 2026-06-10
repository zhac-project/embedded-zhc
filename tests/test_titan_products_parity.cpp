// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Titan Products TPZRCO2HT-Z3 room CO2 / humidity /
// temperature sensor.
//
// Bug fixed (auto-port channel drop): the generated port had been reduced to
// battery-only — fromZigbee:[kFzBattery] and exposes:[battery, voltage]. z2m
// wires fromZigbee:[fz.battery, fz.humidity, fz.temperature, fz.co2] and
// exposes battery_voltage/battery_low/humidity/temperature/co2. This test pins
// that the restored temperature (msTemperatureMeasurement 0x0402), humidity
// (msRelativeHumidity 0x0405) and CO2 (msCO2 0x040D) channels decode to the
// right keys with the right scale, and that the exposes + bindings are present.
//
// z2m-source: titan_products.ts #TPZRCO2HT-Z3.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::titan_products {
extern const PreparedDefinition kDef_TPZRCO2HT_Z3;
}  // namespace zhc::devices::titan_products

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
}

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

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report (fc=0x18 non-mfg, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
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

// Model resolves and the restored full sensor surface is exposed.
void test_exposes_and_bindings() {
    const auto& def = devices::titan_products::kDef_TPZRCO2HT_Z3;
    check(def.model && std::strcmp(def.model, "TPZRCO2HT-Z3") == 0, "model TPZRCO2HT-Z3");

    // Restored sensor channels (dropped by the battery-only auto stub).
    check(def_exposes(def, "battery"),     "exposes battery");
    check(def_exposes(def, "voltage"),     "exposes voltage (battery_voltage)");
    check(def_exposes(def, "battery_low"), "exposes battery_low");
    check(def_exposes(def, "humidity"),    "exposes humidity");
    check(def_exposes(def, "temperature"), "exposes temperature");
    check(def_exposes(def, "co2"),         "exposes co2");

    // from_zigbee must carry all four decoders now (was just battery).
    check(def.from_zigbee_count == 4, "from_zigbee has 4 converters");

    // Bindings: genPowerCfg + temp + co2 on ep1, humidity on ep2.
    check(def_binds(def, 1, 0x0001), "binds genPowerCfg ep1");
    check(def_binds(def, 1, 0x0402), "binds msTemperatureMeasurement ep1");
    check(def_binds(def, 1, 0x040D), "binds msCO2 ep1");
    check(def_binds(def, 2, 0x0405), "binds msRelativeHumidity ep2");
}

// Temperature: msTemperatureMeasurement 0x0402 measuredValue 0x0000 (s16, /100).
// 2305 (0x0901) -> 23.05 °C.
void test_temperature_decode() {
    const auto& def = devices::titan_products::kDef_TPZRCO2HT_Z3;
    const std::array<std::uint8_t, 2> t = {0x01, 0x09};  // 2305 LE
    auto r = dispatch(def, 0x0402, 1,
                      attr_report(0x0000, 0x29,
                                  std::span<const std::uint8_t>(t.data(), t.size())));
    check(has_key(r, "temperature"), "decodes temperature on 0x0402");
    const Value* v = r.merged.find("temperature");
    check(v && v->type == ValueType::Float && v->f > 23.04f && v->f < 23.06f,
          "temperature == 23.05 (s16 /100)");
}

// Humidity: msRelativeHumidity 0x0405 measuredValue 0x0000 (u16, /100), on ep2.
// 5500 (0x157C) -> 55.00 %.
void test_humidity_decode() {
    const auto& def = devices::titan_products::kDef_TPZRCO2HT_Z3;
    const std::array<std::uint8_t, 2> h = {0x7C, 0x15};  // 5500 LE
    auto r = dispatch(def, 0x0405, 2,
                      attr_report(0x0000, 0x21,
                                  std::span<const std::uint8_t>(h.data(), h.size())));
    check(has_key(r, "humidity"), "decodes humidity on 0x0405 ep2");
    const Value* v = r.merged.find("humidity");
    check(v && v->type == ValueType::Float && v->f > 54.99f && v->f < 55.01f,
          "humidity == 55.00 (u16 /100)");
}

// CO2: msCO2 0x040D measuredValue 0x0000 (float, ppm-fraction *1e6).
// z2m fz.co2 -> value*1000000 ppm. 0.0008 -> 800 ppm.
void test_co2_decode() {
    const auto& def = devices::titan_products::kDef_TPZRCO2HT_Z3;
    // IEEE-754 single 0.0008f = 0x3A51B717, LE bytes.
    const float ppm_fraction = 0.0008f;
    std::uint32_t bits;
    std::memcpy(&bits, &ppm_fraction, sizeof(bits));
    const std::array<std::uint8_t, 4> c = {
        static_cast<std::uint8_t>(bits & 0xFF),
        static_cast<std::uint8_t>((bits >> 8) & 0xFF),
        static_cast<std::uint8_t>((bits >> 16) & 0xFF),
        static_cast<std::uint8_t>((bits >> 24) & 0xFF),
    };
    auto r = dispatch(def, 0x040D, 1,
                      attr_report(0x0000, 0x39,  // 0x39 = single-precision float
                                  std::span<const std::uint8_t>(c.data(), c.size())));
    check(has_key(r, "co2"), "decodes co2 on 0x040D");
    const Value* v = r.merged.find("co2");
    check(v && v->type == ValueType::Uint && v->u >= 799 && v->u <= 801,
          "co2 == 800 ppm (float *1e6)");
}

// Battery still decodes: genPowerCfg 0x0001 batteryVoltage 0x0020 (u8, *100 mV).
// 0x1E (30) -> 3000 mV.
void test_battery_decode() {
    const auto& def = devices::titan_products::kDef_TPZRCO2HT_Z3;
    const std::array<std::uint8_t, 1> bv = {0x1E};
    auto r = dispatch(def, 0x0001, 1,
                      attr_report(0x0020, 0x20,
                                  std::span<const std::uint8_t>(bv.data(), bv.size())));
    check(has_key(r, "voltage"), "decodes battery voltage on 0x0001");
    const Value* v = r.merged.find("voltage");
    check(v && v->type == ValueType::Uint && v->u == 3000, "voltage == 3000 mV");
}

}  // namespace

int main() {
    std::printf("test_titan_products_parity\n");
    test_exposes_and_bindings();
    test_temperature_decode();
    test_humidity_decode();
    test_co2_decode();
    test_battery_decode();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
