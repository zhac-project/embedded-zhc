// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the NodOn family. Two real gaps in the hand-rewritten
// ports are pinned here against the z2m wire shape:
//
//   * TRV-4-1-00 declared `trv_mode` (z2m `nodonModernExtend.trvMode`,
//     an enumLookup over hvacThermostat attr 0x4000) and
//     `valve_position` (z2m `.valvePosition`, a numeric over attr
//     0x4001) exposes with matching Tz writers (kTzNodonTrvMode /
//     kTzNodonValvePosition) — but the def only wired the generic
//     kFzThermostat, which decodes 0x0000 / 0x0012 / 0x001C and skips
//     0x4000 / 0x4001. z2m's modernExtends auto-generate a read path, so
//     both exposes were write-only (dead STATE_GET / reporting). Fixed by
//     wiring kFzNodonTrvExtras.
//
//   * SIN-4-RS-20 / SIN-4-RS-20_PRO declared only `position` and wired
//     the generic kFzCoverPosition (lift attr 0x0008 only). z2m's
//     `m.windowCovering({controls:['tilt','lift']})` exposes both
//     `position` and `tilt` and `fz.cover_position_tilt` also decodes
//     attr 0x0009 (CurrentPositionTiltPercentage → `tilt`). Fixed by
//     wiring kFzNodonCoverPositionTilt + a `tilt` expose +
//     kTzCoverPositionTilt writer.
//
// z2m-source: zigbee-herdsman-converters/src/devices/nodon.ts
//             #TRV-4-1-00 / #SIN-4-RS-20[_PRO]
//             + lib/nodon.ts nodonModernExtend
//             + converters/fromZigbee.ts fz.cover_position_tilt.

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

namespace zhc::devices::nodon {
extern const PreparedDefinition kDef_TRV_4_1_00;
extern const PreparedDefinition kDef_SIN_4_RS_20;
extern const PreparedDefinition kDef_SIN_4_RS_20_PRO;
}  // namespace zhc::devices::nodon

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
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
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

// ── TRV-4-1-00: trv_mode (0x4000 enum→string) + valve_position
//    (0x4001 u8, 0..100) decode + regression of the generic surface. ──
void check_trv() {
    const auto& def = devices::nodon::kDef_TRV_4_1_00;

    assert(def_exposes(def, "trv_mode"));
    assert(def_exposes(def, "valve_position"));
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));

    // attr 0x4000 trv_mode, enum8 (type 0x30):
    //   0 → "auto", 1 → "valve_position_mode", 2 → "manual".
    const std::uint8_t m_auto[] = {0x00};
    auto ra = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x4000, 0x30, m_auto));
    assert(ra.any_matched);
    const Value* va = ra.merged.find("trv_mode");
    assert(va && va->type == ValueType::StringRef &&
           std::strcmp(va->str, "auto") == 0);

    const std::uint8_t m_vpm[] = {0x01};
    auto rv = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x4000, 0x30, m_vpm));
    const Value* vv = rv.merged.find("trv_mode");
    assert(vv && std::strcmp(vv->str, "valve_position_mode") == 0);

    const std::uint8_t m_man[] = {0x02};
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x4000, 0x30, m_man));
    const Value* vm = rm.merged.find("trv_mode");
    assert(vm && std::strcmp(vm->str, "manual") == 0);

    // Out-of-range enum (e.g. 5) is dropped (lookup miss).
    const std::uint8_t m_bad[] = {0x05};
    auto rb = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x4000, 0x30, m_bad));
    assert(rb.merged.find("trv_mode") == nullptr);

    // attr 0x4001 valve_position, u8 (type 0x20): 75 → 75 %.
    const std::uint8_t vp[] = {0x4B};
    auto rp = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x4001, 0x20, vp));
    assert(rp.any_matched);
    const Value* pv = rp.merged.find("valve_position");
    assert(pv && pv->type == ValueType::Uint && pv->u == 75);

    // 0 → 0 % (must still emit, not swallowed as falsy).
    const std::uint8_t vz[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x4001, 0x20, vz));
    const Value* pz = rz.merged.find("valve_position");
    assert(pz && pz->type == ValueType::Uint && pz->u == 0);

    // > 100 (e.g. 0xFF) is skipped.
    const std::uint8_t vbad[] = {0xFF};
    auto rvb = dispatch_zcl(def, 0x0201, "hvacThermostat",
                            attr_report(0x4001, 0x20, vbad));
    assert(rvb.merged.find("valve_position") == nullptr);

    // ── Regression: core surface still decodes via generic kFzThermostat ──
    // attr 0x0000 LocalTemperature, s16 = 0x0834 = 2100 → 21.00 °C raw.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // attr 0x001C SystemMode, u8 enum = 4 (heat).
    const std::uint8_t sm[] = {0x04};
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001C, 0x30, sm));
    const Value* sv = rs.merged.find("system_mode");
    assert(sv && sv->type == ValueType::Uint && sv->u == 4);
}

// ── SIN-4-RS-20: position (0x0008) + tilt (0x0009), 0xFF unknown skip. ─
void check_rs(const PreparedDefinition& def) {
    assert(def_exposes(def, "position"));
    assert(def_exposes(def, "tilt"));

    // attr 0x0008 CurrentPositionLiftPercentage, u8 = 40 → position 40.
    const std::uint8_t lift[] = {0x28};
    auto rp = dispatch_zcl(def, 0x0102, "closuresWindowCovering",
                           attr_report(0x0008, 0x20, lift));
    assert(rp.any_matched);
    const Value* pv = rp.merged.find("position");
    assert(pv && pv->type == ValueType::Uint && pv->u == 40);

    // attr 0x0009 CurrentPositionTiltPercentage, u8 = 75 → tilt 75.
    const std::uint8_t tilt[] = {0x4B};
    auto rt = dispatch_zcl(def, 0x0102, "closuresWindowCovering",
                           attr_report(0x0009, 0x20, tilt));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("tilt");
    assert(tv && tv->type == ValueType::Uint && tv->u == 75);

    // 0xFF (255) = unknown → must be skipped (pct > 100).
    const std::uint8_t unknown[] = {0xFF};
    auto ru = dispatch_zcl(def, 0x0102, "closuresWindowCovering",
                           attr_report(0x0009, 0x20, unknown));
    assert(ru.merged.find("tilt") == nullptr);
}

}  // namespace

int main() {
    check_trv();
    check_rs(devices::nodon::kDef_SIN_4_RS_20);
    check_rs(devices::nodon::kDef_SIN_4_RS_20_PRO);
    return 0;
}
