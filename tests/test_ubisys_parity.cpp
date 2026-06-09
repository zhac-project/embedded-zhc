// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Ubisys family. Three real gaps in the
// auto-generated ports are pinned here against the z2m wire shape:
//
//   * H1 / H10 thermostats declared a `pi_heating_demand` (z2m
//     `.withPiHeatingDemand(STATE_GET)`) and `running_mode` (z2m
//     `.withRunningMode(...)`) expose, both decoded by `fz.thermostat`
//     from standard hvacThermostat attrs 0x0008 (PIHeatingDemand) and
//     0x001E (ThermostatRunningMode). The def only wired the generic
//     `kFzThermostat`, which decodes 0x0000 / 0x0012 / 0x001C and skips
//     those two — so they were dead exposes. Fixed by wiring
//     `kFzUbisysThermostatExtras`. Unlike Danfoss, Ubisys does NOT set
//     `dontMapPIHeatingDemand`, so demand is remapped 0-255 → 0-100 %.
//
//   * J1 shutter declared only `position` and wired the generic
//     `kFzCoverPosition` (lift attr 0x0008 only). z2m's
//     `fz.cover_position_tilt` also decodes attr 0x0009
//     (CurrentPositionTiltPercentage → `tilt`), which the J1 reports.
//     Fixed by wiring `kFzUbisysCoverPositionTilt` + a `tilt` expose.
//
//   * R0 is a Zigbee router (z2m `exposes: []`, no toZigbee, no
//     genOnOff). The auto-port wired kFzOnOff/kTzOnOff + a `state`
//     expose — a dead, uncontrollable on/off bundle. Fixed by removing
//     it (mirrors KeenHome GW01).
//
// z2m-source: zigbee-herdsman-converters/src/devices/ubisys.ts
//             #H1 / #H10 / #J1 / #R0 + converters/fromZigbee.ts
//             fz.thermostat / fz.cover_position_tilt.

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

namespace zhc::devices::ubisys {
extern const PreparedDefinition kDef_H1;
extern const PreparedDefinition kDef_H10;
extern const PreparedDefinition kDef_J1;
extern const PreparedDefinition kDef_R0;
}  // namespace zhc::devices::ubisys

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

// ── H1 TRV: pi_heating_demand (0x0008, 0-255 → 0-100 %) + running_mode
//    (0x001E enum→string) + regression of the generic thermostat surface.
void check_h1() {
    const auto& def = devices::ubisys::kDef_H1;

    assert(def_exposes(def, "pi_heating_demand"));
    assert(def_exposes(def, "running_mode"));
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));

    // attr 0x0008 PIHeatingDemand, u8 (type 0x20). Ubisys does NOT set
    // dontMapPIHeatingDemand → mapNumberRange(v, 0, 255, 0, 100) with
    // half-up rounding (v*100+127)/255.
    //   255 → 100 %
    const std::uint8_t full[] = {0xFF};
    auto rf = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, full));
    assert(rf.any_matched);
    const Value* pf = rf.merged.find("pi_heating_demand");
    assert(pf && pf->type == ValueType::Uint && pf->u == 100);

    //   128 → round(128*100/255) = round(50.2) = 50 %
    const std::uint8_t half[] = {0x80};
    auto rh = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, half));
    const Value* ph = rh.merged.find("pi_heating_demand");
    assert(ph && ph->u == 50);

    //   0 → 0 % (must still emit, not swallowed as falsy)
    const std::uint8_t zero[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, zero));
    const Value* pz = rz.merged.find("pi_heating_demand");
    assert(pz && pz->type == ValueType::Uint && pz->u == 0);

    // attr 0x001E ThermostatRunningMode, enum8 (type 0x30): 4 → "heat".
    const std::uint8_t heat[] = {0x04};
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001E, 0x30, heat));
    assert(rm.any_matched);
    const Value* mv = rm.merged.find("running_mode");
    assert(mv && mv->type == ValueType::StringRef &&
           std::strcmp(mv->str, "heat") == 0);

    // 0 → "off".
    const std::uint8_t off[] = {0x00};
    auto ro = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001E, 0x30, off));
    const Value* ov = ro.merged.find("running_mode");
    assert(ov && std::strcmp(ov->str, "off") == 0);

    // Unknown running mode (e.g. 7) is dropped (getFromLookup miss).
    const std::uint8_t bad[] = {0x07};
    auto rb = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001E, 0x30, bad));
    assert(rb.merged.find("running_mode") == nullptr);

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

// ── H10: same pi_heating_demand / running_mode decode wired here too. ──
// H10 carries an endpoint_map (th1..th10 / l1..l10), so per the project
// multi-endpoint convention the runtime keys are suffixed `<key>_<label>`
// for the source endpoint while the EXPOSE names stay bare. Endpoint 1
// maps to label "th1".
void check_h10() {
    const auto& def = devices::ubisys::kDef_H10;
    assert(def_exposes(def, "pi_heating_demand"));   // bare expose name
    assert(def_exposes(def, "running_mode"));

    const std::uint8_t demand[] = {0xFF};  // 255 → 100 %, key suffixed _th1
    auto rd = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, demand));
    assert(rd.any_matched);
    const Value* pd = rd.merged.find("pi_heating_demand_th1");
    assert(pd && pd->type == ValueType::Uint && pd->u == 100);

    const std::uint8_t cool[] = {0x03};    // 3 → "cool", key suffixed _th1
    auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001E, 0x30, cool));
    const Value* cv = rc.merged.find("running_mode_th1");
    assert(cv && cv->type == ValueType::StringRef &&
           std::strcmp(cv->str, "cool") == 0);
}

// ── J1 shutter: position (0x0008) + tilt (0x0009), 255 = unknown skip. ─
void check_j1() {
    const auto& def = devices::ubisys::kDef_J1;
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

    // J1 reports 0xFF (255) when a position is unknown — must be skipped.
    const std::uint8_t unknown[] = {0xFF};
    auto ru = dispatch_zcl(def, 0x0102, "closuresWindowCovering",
                           attr_report(0x0009, 0x20, unknown));
    assert(ru.merged.find("tilt") == nullptr);
}

// ── R0 router: no exposes, no on/off decode (genOnOff frame ignored). ──
void check_r0() {
    const auto& def = devices::ubisys::kDef_R0;
    assert(def.exposes_count == 0);
    assert(def.from_zigbee_count == 0);
    assert(def.to_zigbee_count == 0);
    assert(!def_exposes(def, "state"));

    // A genOnOff (0x0006) attr report must produce no `state` (no Fz
    // wired). Dispatch should simply not match.
    const std::uint8_t on[] = {0x01};
    auto r = dispatch_zcl(def, 0x0006, "genOnOff",
                          attr_report(0x0000, 0x10, on));
    assert(r.merged.find("state") == nullptr);
}

}  // namespace

int main() {
    check_h1();
    check_h10();
    check_j1();
    check_r0();
    return 0;
}
