// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Tapestry THPZ1 occupancy + temperature + humidity
// sensor. The auto-port mis-classified it as an IAS-zone device:
//
//   z2m (tapestry.ts #THPZ1):
//     fromZigbee=[fz.temperature, fz.humidity, fz.occupancy]
//     exposes=[e.occupancy(), e.temperature(), e.humidity()]
//
// The port wired the generic kFzIasZone (cluster 0x0500) and emitted a
// phantom alarm/tamper/battery_low bundle bound to 0x0500, dropping ALL
// three real channels (temperature 0x0402, humidity 0x0405, occupancy
// 0x0406). Re-ported to the three generic measurement decoders. This pins
// the fix.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tapestry {
extern const PreparedDefinition kDef_THPZ1;
}  // namespace zhc::devices::tapestry

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

bool def_uses_fz(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == conv) return true;
    return false;
}

DispatchResult dispatch_zcl(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── THPZ1: tri-channel sensor; no phantom IAS bundle. ───────────────────────
void test_thpz1_shape() {
    using namespace zhc::devices::tapestry;
    const auto& def = kDef_THPZ1;

    // Real channels present.
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));

    // Phantom IAS bundle gone.
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "tamper"));
    assert(!def_exposes(def, "battery_low"));

    // Correct converters wired; IAS converter not wired.
    assert(def_uses_fz(def, &::zhc::generic::kFzTemperature));
    assert(def_uses_fz(def, &::zhc::generic::kFzHumidity));
    assert(def_uses_fz(def, &::zhc::generic::kFzOccupancy));
    assert(!def_uses_fz(def, &::zhc::generic::kFzIasZone));

    // Correct binds; phantom 0x0500 bind gone.
    assert(def_binds(def, 1, 0x0402));
    assert(def_binds(def, 1, 0x0405));
    assert(def_binds(def, 1, 0x0406));
    assert(!def_binds(def, 1, 0x0500));
}

void test_thpz1_decode() {
    using namespace zhc::devices::tapestry;
    const auto& def = kDef_THPZ1;

    // msTemperatureMeasurement measuredValue (0x0000, s16, /100) -> temperature.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> t = {0xC4, 0x09};   // 2500 LE -> 25.00 °C
    auto tr = dispatch_zcl(c1, def, 0x0402, "msTemperatureMeasurement",
                           attr_report(0x0000, 0x29,
                                       std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    assert(tr.merged.find("temperature") != nullptr);

    // msRelativeHumidity measuredValue (0x0000, u16, /100) -> humidity.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> h = {0x10, 0x17};   // 5904 LE -> 59.04 %
    auto hr = dispatch_zcl(c2, def, 0x0405, "msRelativeHumidity",
                           attr_report(0x0000, 0x21,
                                       std::span<const std::uint8_t>(h.data(), h.size())));
    assert(hr.any_matched);
    assert(hr.merged.find("humidity") != nullptr);

    // msOccupancySensing occupancy (0x0000, bitmap8, bit0) -> occupancy.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 1> o = {0x01};         // occupied
    auto orr = dispatch_zcl(c3, def, 0x0406, "msOccupancySensing",
                            attr_report(0x0000, 0x18,
                                        std::span<const std::uint8_t>(o.data(), o.size())));
    assert(orr.any_matched);
    const Value* ov = orr.merged.find("occupancy");
    assert(ov != nullptr);
    assert(ov->type == ValueType::Bool && ov->b == true);
}

}  // namespace

int main() {
    test_thpz1_shape();
    test_thpz1_decode();
    return 0;
}
