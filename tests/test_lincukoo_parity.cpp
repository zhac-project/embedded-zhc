// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Lincukoo SZT06 temperature/humidity sensor.
// z2m-source: lincukoo.ts #SZT06 —
//   extend: [m.temperature(), m.humidity(), m.identify({isSleepy:true}),
//            m.battery({voltage:true})]
//
// Bug fixed: the generated SZT06 stub lowered only kFzBattery and exposed
// only battery/voltage, so a paired sensor surfaced no temperature or
// humidity despite reporting both on the standard msTemperatureMeasurement
// (0x0402) and msRelativeHumidity (0x0405) clusters. The def was graduated
// to a Tier-2 parent override that adds kFzTemperature + kFzHumidity (which
// emit the "temperature"/"humidity" keys) plus the 0x0402 / 0x0405
// reporting bindings.
//
// These tests pin, on real ZCL attribute-report wire shapes, that SZT06
// now decodes temperature and humidity (and that the channels are declared
// as exposes with matching reporting bindings). The rest of the Lincukoo
// vendor is Tuya-DP and already wires its tuyaDatapoints; the named
// model stubs (SZLR08, SZR07, E02C-Z10T, …) are harmless dead duplicates
// of their manufacturer-discriminated kDefLin__TZE* twins, which win in
// find_definition() Pass 1 (Tuya-styled, manufacturer-gated).

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

namespace zhc::devices::lincukoo {
extern const PreparedDefinition kDef_SZT06;   // temperature + humidity + battery
}  // namespace zhc::devices::lincukoo

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

bool f_near(const Value* v, float expect) {
    if (!v || v->type != ValueType::Float) return false;
    const float d = v->f - expect;
    return (d < 0.01f) && (d > -0.01f);
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

// msTemperatureMeasurement (0x0402) attr 0x0000 s16 -> temperature (/100).
static void test_szt06_temperature() {
    const auto& def = devices::lincukoo::kDef_SZT06;
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x0402));
    const std::uint8_t val[] = {0xF4, 0x08};   // 0x08F4 = 2292 -> 22.92 °C
    auto r = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 1,
                          attr_report(0x0000, 0x29, val));
    assert(r.any_matched);
    assert(f_near(r.merged.find("temperature"), 22.92f));
}

// msRelativeHumidity (0x0405) attr 0x0000 u16 -> humidity (/100).
static void test_szt06_humidity() {
    const auto& def = devices::lincukoo::kDef_SZT06;
    assert(def_exposes(def, "humidity"));
    assert(def_binds(def, 0x0405));
    const std::uint8_t val[] = {0x10, 0x17};   // 0x1710 = 5904 -> 59.04 %
    auto r = dispatch_zcl(def, 0x0405, "msRelativeHumidity", 1,
                          attr_report(0x0000, 0x21, val));
    assert(r.any_matched);
    assert(f_near(r.merged.find("humidity"), 59.04f));
}

// Battery channel must remain (z2m m.battery({voltage:true})), and the
// genPowerCfg binding must be preserved alongside the new measurement ones.
static void test_szt06_battery_retained() {
    const auto& def = devices::lincukoo::kDef_SZT06;
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    assert(def_binds(def, 0x0001));
}

int main() {
    test_szt06_temperature();
    test_szt06_humidity();
    test_szt06_battery_retained();
    return 0;
}
