// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Wirenboard WB-MSW-ZIGBEE multi-sensor family
// (v.3 + v.4). Both ports declared `occupancy` and `co2` exposes and
// bound the matching standard clusters (msOccupancySensing 0x0406,
// msCO2 0x040D), but neither wired an fz converter for them — so those
// two channels were dead against the z2m wire shape:
//
//   * occupancy — z2m wires fz.occupancy (msOccupancySensing attr
//     0x0000 bitmap8, bit 0). Fixed by wiring the generic kFzOccupancy.
//
//   * co2 — z2m wires fz.co2 (msCO2 attr 0x0000 single-precision float,
//     `co2 = floor(measuredValue * 1e6)` → ppm). No generic CO2 decoder
//     existed; added kFzCO2 to definitions/_generic/_shared.{hpp,cpp}
//     and wired it here.
//
// The temperature / humidity (both ports) and illuminance (v.4 only)
// surfaces were already decoded by the generic kFzTemperature /
// kFzHumidity / kFzIlluminance and are regression-checked here too.
// The sprut-custom-cluster channels (noise, voc) have no generic
// decoder yet (deferred infra) and are intentionally not asserted.
//
// z2m-source: zigbee-herdsman-converters/src/devices/wirenboard.ts
//             #WB-MSW-ZIGBEE v.3 / v.4 +
//             converters/fromZigbee.ts fz.occupancy / fz.co2 / fz.humidity.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::wirenboard {
extern const PreparedDefinition kDef_WB_MSW_ZIGBEE_v_3;
extern const PreparedDefinition kDef_WB_MSW_ZIGBEE_v_4;
}  // namespace zhc::devices::wirenboard

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

// IEEE-754 little-endian bytes for a float literal.
std::array<std::uint8_t, 4> f32_le(float f) {
    std::array<std::uint8_t, 4> out{};
    std::memcpy(out.data(), &f, 4);
    return out;
}

// ── Shared sensor-surface checks (run against both v.3 and v.4) ──────
//
// `suffix` accounts for the endpoint-map key rewrite: v.4 declares an
// `endpoint_map`, so dispatch rewrites every emitted key on endpoint 1
// to `<key>_default` (see MEMORY: multi-endpoint key suffixing). v.3
// has no endpoint_map, so its keys stay bare. The exposes table always
// uses the bare names; the runtime merged-payload keys carry the
// suffix. We look up the runtime key with the suffix applied.
void check_common(const PreparedDefinition& def, const char* suffix) {
    auto runtime_key = [&](const char* base, char* buf, std::size_t n) {
        std::snprintf(buf, n, "%s%s", base, suffix);
        return buf;
    };
    char kb[64];

    // Exposes carry the bare names regardless of endpoint suffixing.
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "co2"));

    // ── Parity fix 1: occupancy from msOccupancySensing attr 0x0000
    //    map8 bit 0 (z2m fz.occupancy). Converter is endpoint-agnostic. ─
    const std::uint8_t occ_on[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0406, "msOccupancySensing", 1,
                           attr_report(0x0000, 0x18, occ_on));
    assert(ro.any_matched);
    const Value* ov = ro.merged.find(runtime_key("occupancy", kb, sizeof(kb)));
    assert(ov && ov->type == ValueType::Bool && ov->b == true);

    const std::uint8_t occ_off[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0406, "msOccupancySensing", 1,
                           attr_report(0x0000, 0x18, occ_off));
    const Value* oz = rz.merged.find(runtime_key("occupancy", kb, sizeof(kb)));
    assert(oz && oz->type == ValueType::Bool && oz->b == false);

    // ── Parity fix 2: co2 from msCO2 attr 0x0000 single-prec float
    //    (z2m fz.co2 = floor(measuredValue * 1e6)). 0.0004 → 400 ppm. ──
    auto co2_bytes = f32_le(0.0004f);
    auto rc = dispatch_zcl(def, 0x040D, "msCO2", 1,
                           attr_report(0x0000, 0x39, co2_bytes));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find(runtime_key("co2", kb, sizeof(kb)));
    assert(cv && cv->type == ValueType::Uint);
    // float→ppm rounding: floor(0.0004 * 1e6) == 400 (allow ±1 for fp).
    assert(cv->u == 400u || cv->u == 399u || cv->u == 401u);

    // ── Regression: temperature (msTemperatureMeasurement, s16 /100). ──
    const std::uint8_t temp[] = {0x34, 0x08};   // 0x0834 = 2100 → raw 2100
    auto rt = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 1,
                           attr_report(0x0000, 0x29, temp));
    assert(rt.any_matched);
    assert(rt.merged.find(runtime_key("temperature", kb, sizeof(kb))) != nullptr);

    // ── Regression: humidity (msRelativeHumidity, u16 /100). ──
    const std::uint8_t hum[] = {0x88, 0x13};    // 0x1388 = 5000 → raw 5000
    auto rh = dispatch_zcl(def, 0x0405, "msRelativeHumidity", 1,
                           attr_report(0x0000, 0x21, hum));
    assert(rh.any_matched);
    assert(rh.merged.find(runtime_key("humidity", kb, sizeof(kb))) != nullptr);
}

void check_v3() {
    const auto& def = devices::wirenboard::kDef_WB_MSW_ZIGBEE_v_3;
    // v.3 has no endpoint_map → runtime keys are bare.
    check_common(def, "");
    // v.3 has no illuminance channel.
    assert(!def_exposes(def, "illuminance"));
    // Relays present.
    assert(def_exposes(def, "state_l1"));
    assert(def_exposes(def, "state_l2"));
    assert(def_exposes(def, "state_l3"));
}

void check_v4() {
    const auto& def = devices::wirenboard::kDef_WB_MSW_ZIGBEE_v_4;
    // v.4 declares an endpoint_map → endpoint-1 keys gain `_default`.
    check_common(def, "_default");

    // ── v.4 adds illuminance (msIlluminanceMeasurement 0x0400, raw lx). ─
    assert(def_exposes(def, "illuminance"));
    const std::uint8_t lux[] = {0xE8, 0x03};   // 0x03E8 = 1000 raw lux
    auto rl = dispatch_zcl(def, 0x0400, "msIlluminanceMeasurement", 1,
                           attr_report(0x0000, 0x21, lux));
    assert(rl.any_matched);
    const Value* lv = rl.merged.find("illuminance_default");
    assert(lv && lv->type == ValueType::Uint && lv->u == 1000u);
}

}  // namespace

int main() {
    check_v3();
    check_v4();
    return 0;
}
