// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the TNCE TNCE_CLIMATE temperature & humidity sensor
// (TELINK TLSR82xx end-device).
//
// Bug fixed (missing-decoder):
//   z2m wires m.temperature() + m.humidity() + m.battery() and the fingerprint
//   declares inputClusters [0,3,32,1026,1029,1] (1026=msTemperatureMeasurement,
//   1029=msRelativeHumidity, 1=genPowerCfg). The auto-port lowered only
//   kFzBattery, dropping BOTH environmental channels (decoders, exposes and
//   bindings). Added kFzTemperature (0x0402) + kFzHumidity (0x0405).
//
// z2m-source: tnce.ts #TNCE_CLIMATE.

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tnce {
extern const PreparedDefinition kDef_TNCE_CLIMATE;
}  // namespace zhc::devices::tnce

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

// Keep `ctx` alive while the result is read.
DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── TNCE_CLIMATE: temperature (0x0402) + humidity (0x0405) regained. ───────
void test_tnce_climate() {
    using namespace zhc::devices::tnce;
    const auto& def = kDef_TNCE_CLIMATE;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 1, 0x0001));   // genPowerCfg (battery) kept
    assert(def_binds(def, 1, 0x0402));   // msTemperatureMeasurement added
    assert(def_binds(def, 1, 0x0405));   // msRelativeHumidity added

    // 21.50 °C -> 2150 (s16) on msTemperatureMeasurement measuredValue 0x0000.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> t = {0x66, 0x08};   // 2150 LE
    auto tr = dispatch_zcl_ep(c1, def, 0x0402, 1, "msTemperatureMeasurement",
                              attr_report(0x0000, 0x29, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    // fz_temperature scales raw/100 -> Float 21.5.
    const Value* tv = tr.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);

    // 55.00 % -> 5500 (u16) on msRelativeHumidity measuredValue 0x0000.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> h = {0x7C, 0x15};   // 5500 LE
    auto hr = dispatch_zcl_ep(c2, def, 0x0405, 1, "msRelativeHumidity",
                              attr_report(0x0000, 0x21, std::span<const std::uint8_t>(h.data(), h.size())));
    assert(hr.any_matched);
    assert(hr.merged.find("humidity") != nullptr);
}

}  // namespace

int main() {
    test_tnce_climate();
    return 0;
}
