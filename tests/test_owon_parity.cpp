// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Owon HVAC family. The auto-generated thermostat
// ports declared exposes that the generic `kFzThermostat` (which decodes
// only hvacThermostat attrs 0x0000 / 0x0012 / 0x001C) never filled, and
// the fan-coils declared an `occupancy` expose with no decoder wired —
// so several exposes were dead against the z2m wire shape:
//
//   * AC201 / AC221 / PCT504 declare `current_cooling_setpoint`
//     (z2m `.withSetpoint("occupied_cooling_setpoint", ...)`), decoded
//     by `fz.thermostat` from attr 0x0011 (OccupiedCoolingSetpoint).
//     Fixed by wiring `kFzOwonThermostatExtras`.
//
//   * PCT504 / PCT512 declare `running_state` (z2m `.withRunningState`),
//     decoded by `fz.thermostat` from attr 0x0029 (ThermostatRunningState)
//     via `constants.thermostatRunningStates`. Fixed by the same extras
//     converter (enum → string: 0=idle, 1=heat, 2=cool, 4=fan_only).
//
//   * PCT504 / PCT512 declare `occupancy` and bind msOccupancySensing,
//     but wired no decoder (the def TODOs claimed kFzOccupancy was "not
//     yet in _shared" — it is). Fixed by wiring the generic
//     `kFzOccupancy` (attr 0x0000 bit 0).
//
// The core thermostat surface (local_temperature / current_heating_
// setpoint / system_mode) is still decoded by the generic kFzThermostat
// wired alongside, and is regression-checked here too.
//
// z2m-source: zigbee-herdsman-converters/src/devices/owon.ts
//             #AC201 / #AC221 / #PCT504 / #PCT512 +
//             converters/fromZigbee.ts fz.thermostat / fz.occupancy +
//             lib/constants.ts thermostatRunningStates.

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

namespace zhc::devices::owon {
extern const PreparedDefinition kDef_AC201;
extern const PreparedDefinition kDef_AC221;
extern const PreparedDefinition kDef_PCT504;
extern const PreparedDefinition kDef_PCT512;
}  // namespace zhc::devices::owon

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

bool streq(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

// ── PCT504 fan-coil: cooling setpoint + running_state + occupancy ─────
void check_pct504() {
    const auto& def = devices::owon::kDef_PCT504;

    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "current_cooling_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "occupancy"));

    // ── Parity fix 1: attr 0x0011 OccupiedCoolingSetpoint, s16 =
    //    0x0898 = 2200 → current_cooling_setpoint (raw, runtime /100). ──
    const std::uint8_t cool[] = {0x98, 0x08};
    auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0011, 0x29, cool));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("current_cooling_setpoint");
    assert(cv && cv->type == ValueType::Int && cv->i == 2200);

    // ── Parity fix 2: attr 0x0029 ThermostatRunningState, map16. ──
    //   2 → "cool"
    const std::uint8_t rs_cool[] = {0x02, 0x00};
    auto rr = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0029, 0x19, rs_cool));
    assert(rr.any_matched);
    assert(streq(rr.merged.find("running_state"), "cool"));
    //   0 → "idle"
    const std::uint8_t rs_idle[] = {0x00, 0x00};
    auto ri = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0029, 0x19, rs_idle));
    assert(streq(ri.merged.find("running_state"), "idle"));

    // ── Parity fix 3: occupancy from msOccupancySensing (ep2), attr
    //    0x0000 map8 bit 0. Converter is endpoint-agnostic (WILDCARD). ──
    const std::uint8_t occ_on[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0406, "msOccupancySensing", 2,
                           attr_report(0x0000, 0x18, occ_on));
    assert(ro.any_matched);
    const Value* ov = ro.merged.find("occupancy");
    assert(ov && ov->type == ValueType::Bool && ov->b == true);

    const std::uint8_t occ_off[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0406, "msOccupancySensing", 2,
                           attr_report(0x0000, 0x18, occ_off));
    const Value* oz = rz.merged.find("occupancy");
    assert(oz && oz->type == ValueType::Bool && oz->b == false);

    // ── Regression: core surface still decodes via generic kFzThermostat ──
    const std::uint8_t lt[] = {0x34, 0x08};   // 0x0834 = 2100 → 21.00 °C raw
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    const std::uint8_t sm[] = {0x04};         // system_mode = 4 (heat)
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 4);
}

// ── PCT512 thermostat: running_state + occupancy (no cooling expose) ──
void check_pct512() {
    const auto& def = devices::owon::kDef_PCT512;

    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "system_mode"));
    // PCT512 is heat-only: it must NOT carry a cooling-setpoint expose.
    assert(!def_exposes(def, "current_cooling_setpoint"));

    //   1 → "heat"
    const std::uint8_t rs_heat[] = {0x01, 0x00};
    auto rr = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0029, 0x19, rs_heat));
    assert(rr.any_matched);
    assert(streq(rr.merged.find("running_state"), "heat"));

    // occupancy bit 0 set → true (msOccupancySensing on ep1 for PCT512).
    const std::uint8_t occ_on[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0406, "msOccupancySensing", 1,
                           attr_report(0x0000, 0x18, occ_on));
    assert(ro.any_matched);
    const Value* ov = ro.merged.find("occupancy");
    assert(ov && ov->type == ValueType::Bool && ov->b == true);

    // Regression: heating setpoint still decodes via generic kFzThermostat.
    const std::uint8_t sp[] = {0xD0, 0x07};   // 0x07D0 = 2000
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0012, 0x29, sp));
    const Value* sv = rs.merged.find("current_heating_setpoint");
    assert(sv && sv->type == ValueType::Int && sv->i == 2000);
}

// ── AC201 / AC221 HVAC controllers: cooling setpoint decode ───────────
void check_ac_controllers() {
    for (const PreparedDefinition* defp :
         {&devices::owon::kDef_AC201, &devices::owon::kDef_AC221}) {
        const auto& def = *defp;
        assert(def_exposes(def, "current_cooling_setpoint"));
        assert(def_exposes(def, "current_heating_setpoint"));
        assert(def_exposes(def, "system_mode"));

        // attr 0x0011 OccupiedCoolingSetpoint, s16 = 0x0708 = 1800 → 18 °C.
        const std::uint8_t cool[] = {0x08, 0x07};
        auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                               attr_report(0x0011, 0x29, cool));
        assert(rc.any_matched);
        const Value* cv = rc.merged.find("current_cooling_setpoint");
        assert(cv && cv->type == ValueType::Int && cv->i == 1800);

        // Regression: system_mode via generic kFzThermostat.
        const std::uint8_t sm[] = {0x03};   // cool
        auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                               attr_report(0x001C, 0x30, sm));
        const Value* mv = rm.merged.find("system_mode");
        assert(mv && mv->type == ValueType::Uint && mv->u == 3);
    }
}

}  // namespace

int main() {
    check_pct504();
    check_pct512();
    check_ac_controllers();
    return 0;
}
