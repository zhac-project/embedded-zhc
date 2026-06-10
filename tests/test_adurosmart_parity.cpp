// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the AduroSmart ERIA family.
//
// AduroSmart is mostly stock m.light()/m.onOff() bulbs + relays (clean
// against z2m, exercised by the generic suites) plus a remote, a wireless
// dimmer and a siren (all already hand-ported with the correct action /
// IAS surfaces). The real z2m<->ZHC gap was on the THREE power-measuring
// devices:
//
//   * 81848 (AD-SmartPlug3001) — z2m fz.on_off + fz.electrical_measurement,
//       exposes [switch, power, current, voltage]. NO energy, NO metering.
//   * 81998 (ONOFF_METER_RELAY) — z2m m.onOff() + m.electricityMeter
//       ({cluster:"electrical"}).
//   * 81949 (DimmerM3002) — z2m m.light() + m.electricityMeter
//       ({cluster:"electrical"}) (+ vendor dimmer config extends, INFRA).
//
// z2m's electricityMeter({cluster:"electrical"}) reads ONLY the
// haElectricalMeasurement cluster (0x0B04) → power/voltage/current and
// exposes NO `energy` (genericMeter electrical branch, modernExtend.ts
// L2296-2322; it does NOT read seMetering 0x0702). The auto-generated
// ports used a shared "EM" bundle that ALSO wired the generic kFzMetering
// (0x0702) decoder, declared a phantom `energy` (kWh) expose and bound
// 0x0702 — none of which z2m surfaces.
//
// Fixed by making the kFzAdu*EM bundles electrical-only (kFzElectrical-
// Measurement only; no kFzMetering; no `energy` expose; no 0x0702 bind).
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * each EM device decodes power/voltage/current from 0x0B04,
//   * no device exposes `energy`,
//   * no device binds seMetering 0x0702 (but all bind 0x0B04),
//   * the on/off + dimmer surfaces survive (state / brightness).
//
// z2m-source: zigbee-herdsman-converters/src/devices/adurosmart.ts
//             #81848 / #81998 / #81949 + lib/modernExtend.ts
//             electricityMeter / genericMeter.

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

namespace zhc::devices::adurosmart {
extern const PreparedDefinition kDef_D81848;
extern const PreparedDefinition kDef_D81998;
extern const PreparedDefinition kDef_D81949;
}  // namespace zhc::devices::adurosmart

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

// ZCL attribute report builder (fc=0x18, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// haElectricalMeasurement (0x0B04): ActivePower 0x050B (s16), RMSVoltage
// 0x0505 (u16), RMSCurrent 0x0508 (u16). Decoded by kFzElectricalMeasurement
// to power/voltage/current.
void check_electrical(const PreparedDefinition& def) {
    // ActivePower 0x050B (s16) = 230 → power.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> pw = {0xE6, 0x00};  // 230
    auto p = dispatch_zcl_ep(c1, def, 0x0B04, 1, "haElectricalMeasurement",
                             attr_report(0x050B, 0x29,
                                         std::span<const std::uint8_t>(pw.data(), pw.size())));
    assert(p.any_matched);
    const Value* pv = p.merged.find("power");
    assert(pv && (pv->type == ValueType::Int || pv->type == ValueType::Uint));

    // RMSVoltage 0x0505 (u16) = 2300 → voltage.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> vo = {0xFC, 0x08};  // 2300
    auto vr = dispatch_zcl_ep(c2, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0505, 0x21,
                                          std::span<const std::uint8_t>(vo.data(), vo.size())));
    assert(vr.any_matched);
    const Value* vv = vr.merged.find("voltage");
    assert(vv && vv->type == ValueType::Uint && vv->u == 2300);

    // RMSCurrent 0x0508 (u16) = 150 → current.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 2> cu = {0x96, 0x00};  // 150
    auto cr = dispatch_zcl_ep(c3, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0508, 0x21,
                                          std::span<const std::uint8_t>(cu.data(), cu.size())));
    assert(cr.any_matched);
    const Value* cv = cr.merged.find("current");
    assert(cv && cv->type == ValueType::Uint && cv->u == 150);
}

// Shared invariants for every electrical-only EM device: power/voltage/
// current exposed, NO phantom `energy`, binds 0x0B04 NOT 0x0702.
void check_em_shape(const PreparedDefinition& def) {
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(!def_exposes(def, "energy"));    // electrical-only → no energy
    assert(def_binds(def, 1, 0x0B04));      // haElectricalMeasurement
    assert(!def_binds(def, 1, 0x0702));     // NOT seMetering
    check_electrical(def);
}

// ── 81848: AD-SmartPlug3001 — on/off + electrical-only. ──────────────────
void check_81848() {
    using namespace zhc::devices::adurosmart;
    const auto& def = kDef_D81848;
    assert(def_exposes(def, "state"));      // m.onOff() switch survives
    assert(!def_exposes(def, "brightness"));
    assert(def_binds(def, 1, 0x0006));
    check_em_shape(def);
}

// ── 81998: ONOFF_METER_RELAY — on/off + electrical-only. ─────────────────
void check_81998() {
    using namespace zhc::devices::adurosmart;
    const auto& def = kDef_D81998;
    assert(def_exposes(def, "state"));
    assert(!def_exposes(def, "brightness"));
    assert(def_binds(def, 1, 0x0006));
    check_em_shape(def);
}

// ── 81949: DimmerM3002 — dimmable light + electrical-only. ────────────────
void check_81949() {
    using namespace zhc::devices::adurosmart;
    const auto& def = kDef_D81949;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));  // m.light() dimmer survives
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));       // genLevelCtrl

    // Brightness decode survives (genLevelCtrl currentLevel attr 0x0000).
    RuntimeContext c{};
    const std::array<std::uint8_t, 1> lv = {0x7F};  // 127
    auto br = dispatch_zcl_ep(c, def, 0x0008, 1, "genLevelCtrl",
                              attr_report(0x0000, 0x20,
                                          std::span<const std::uint8_t>(lv.data(), lv.size())));
    assert(br.any_matched);
    const Value* bv = br.merged.find("brightness");
    assert(bv && bv->type == ValueType::Uint && bv->u == 127);

    check_em_shape(def);
}

}  // namespace

int main() {
    check_81848();
    check_81998();
    check_81949();
    return 0;
}
