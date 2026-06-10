// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Simpla Home Soil Pro soil-moisture sensor.
// z2m-source: zigbee-herdsman-converters/src/devices/simpla_home.ts.
//
// Bug fixed (MISSING decoder/channel + wrong endpoint_map):
//   The auto-generated def was a battery-only stub. z2m's Soil Pro is a
//   multi-sensor exposing temperature (msTemperatureMeasurement 0x0402)
//   and illuminance (msIlluminanceMeasurement 0x0400) on the default
//   endpoint, plus TWO soil_moisture channels (msSoilMoisture 0x0408) on
//   the z1_top (ep 2) and z2_bottom (ep 3) endpoints, plus battery. The
//   stub dropped temperature, illuminance and both soil zones, and placed
//   both soil endpoints at ep 1 (which would have collided on a bare
//   "soil_moisture" key).
//
//   The graduated def now wires kFzTemperature / kFzSoilMoisture /
//   kFzIlluminance / kFzBattery and corrects the endpoint_map to
//   {z1_top: 2, z2_bottom: 3} so the two zones decode to suffixed runtime
//   keys soil_moisture_z1_top / soil_moisture_z2_bottom while
//   temperature/illuminance (endpoint 1, not in the map) stay bare.

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

namespace zhc::devices::simpla_home {
extern const PreparedDefinition kDef_Soil_Pro;
}  // namespace zhc::devices::simpla_home

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool f_near(const Value* v, float expect) {
    if (!v || v->type != ValueType::Float) return false;
    const float d = v->f - expect;
    return (d < 0.01f) && (d > -0.01f);
}

bool u_eq(const Value* v, std::uint64_t expect) {
    return v && v->type == ValueType::Uint && v->u == expect;
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

// u16 little-endian helper.
std::array<std::uint8_t, 2> le16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x & 0xFF), static_cast<std::uint8_t>(x >> 8)};
}

const PreparedDefinition& def() { return zhc::devices::simpla_home::kDef_Soil_Pro; }

}  // namespace

// The restored multi-sensor channels are declared as exposes; the stub was
// battery-only.
static void test_restored_exposes_declared() {
    const auto& d = def();
    assert(def_exposes(d, "temperature"));
    assert(def_exposes(d, "soil_moisture"));
    assert(def_exposes(d, "illuminance"));
    assert(def_exposes(d, "battery"));
    assert(def_exposes(d, "voltage"));
}

// Bindings: temperature/illuminance on ep 1, soil on ep 2 + ep 3, power
// config on ep 1.
static void test_bindings_present() {
    const auto& d = def();
    assert(def_binds(d, 1, 0x0402));   // temperature
    assert(def_binds(d, 1, 0x0400));   // illuminance
    assert(def_binds(d, 2, 0x0408));   // soil z1_top
    assert(def_binds(d, 3, 0x0408));   // soil z2_bottom
    assert(def_binds(d, 1, 0x0001));   // battery / power cfg
}

// endpoint_map carries ONLY the two soil zones; endpoint 1 (temperature /
// illuminance) is intentionally absent so those keys stay bare.
static void test_endpoint_map_soil_zones_only() {
    const auto& d = def();
    assert(d.endpoint_map_count == 2);
    bool z1 = false, z2 = false, ep1 = false;
    for (std::size_t i = 0; i < d.endpoint_map_count; ++i) {
        if (d.endpoint_map[i].endpoint == 2 &&
            std::strcmp(d.endpoint_map[i].label, "z1_top") == 0) z1 = true;
        if (d.endpoint_map[i].endpoint == 3 &&
            std::strcmp(d.endpoint_map[i].label, "z2_bottom") == 0) z2 = true;
        if (d.endpoint_map[i].endpoint == 1) ep1 = true;
    }
    assert(z1 && z2 && !ep1);
}

// soil_moisture on ep 2 -> suffixed key soil_moisture_z1_top, value /100.
static void test_soil_z1_top_suffixed() {
    auto bytes = le16(4500);  // 45.00 %
    auto rep = attr_report(0x0000, 0x21, std::span<const std::uint8_t>(bytes.data(), 2));
    auto r = dispatch_zcl(def(), 0x0408, "msSoilMoisture", /*src_ep=*/2,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    assert(f_near(r.merged.find("soil_moisture_z1_top"), 45.0f));
    assert(r.merged.find("soil_moisture") == nullptr);          // bare key gone
    assert(r.merged.find("soil_moisture_z2_bottom") == nullptr);
}

// soil_moisture on ep 3 -> suffixed key soil_moisture_z2_bottom.
static void test_soil_z2_bottom_suffixed() {
    auto bytes = le16(1234);  // 12.34 %
    auto rep = attr_report(0x0000, 0x21, std::span<const std::uint8_t>(bytes.data(), 2));
    auto r = dispatch_zcl(def(), 0x0408, "msSoilMoisture", /*src_ep=*/3,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    assert(f_near(r.merged.find("soil_moisture_z2_bottom"), 12.34f));
    assert(r.merged.find("soil_moisture_z1_top") == nullptr);
}

// temperature on ep 1 stays bare (ep 1 not in the endpoint_map), value /100.
static void test_temperature_bare_on_ep1() {
    auto bytes = le16(static_cast<std::uint16_t>(2350));  // 23.50 °C
    auto rep = attr_report(0x0000, 0x29, std::span<const std::uint8_t>(bytes.data(), 2));
    auto r = dispatch_zcl(def(), 0x0402, "msTemperatureMeasurement", /*src_ep=*/1,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    assert(f_near(r.merged.find("temperature"), 23.5f));
    assert(r.merged.find("temperature_z1_top") == nullptr);
}

// illuminance on ep 1 stays bare, passed through as raw u16.
static void test_illuminance_bare_on_ep1() {
    auto bytes = le16(static_cast<std::uint16_t>(789));
    auto rep = attr_report(0x0000, 0x21, std::span<const std::uint8_t>(bytes.data(), 2));
    auto r = dispatch_zcl(def(), 0x0400, "msIlluminanceMeasurement", /*src_ep=*/1,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    assert(u_eq(r.merged.find("illuminance"), 789));
}

int main() {
    test_restored_exposes_declared();
    test_bindings_present();
    test_endpoint_map_soil_zones_only();
    test_soil_z1_top_suffixed();
    test_soil_z2_bottom_suffixed();
    test_temperature_bare_on_ep1();
    test_illuminance_bare_on_ep1();
    return 0;
}
