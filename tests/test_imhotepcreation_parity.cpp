// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Imhotep Creation thermostat family. Both ported
// defs declared (or, for BRI4P, should have declared) standard
// hvacThermostat setpoint-limit / cooling-setpoint exposes that the
// generic `kFzThermostat` (which decodes only hvacThermostat attrs
// 0x0000 / 0x0012 / 0x001C) never filled — dead against z2m's wire shape:
//
//   * E-Ctrl (heater thermostat + whitelabels RSS/RPH E-Ctrl) declared
//     `min_heat_setpoint_limit` / `max_heat_setpoint_limit`
//     (z2m `.numeric(... ea.ALL)`), decoded by `fz.thermostat` from attrs
//     0x0015 / 0x0016 — but no decoder was wired, so both were dead.
//
//   * BRI4P (16-channel underfloor-heating bridge) carried only
//     local_temperature / current_heating_setpoint / system_mode, while
//     z2m exposes per endpoint a cooling setpoint (0x0011) plus min/max
//     heat AND cool setpoint limits (0x0015-0x0018), all via fz.thermostat.
//     Those exposes + decoder were entirely missing.
//
// Fixed by wiring a vendor-local `kFzImhotepThermostatExtras` (attrs
// 0x0011 / 0x0015 / 0x0016 / 0x0017 / 0x0018, raw s16 pass-through,
// runtime /100 downstream) alongside the generic kFzThermostat, and by
// adding the missing BRI4P exposes. The core surface (local_temperature
// / current_heating_setpoint / system_mode) is still decoded by the
// generic converter and regression-checked here.
//
// BRI4P opts into per-endpoint dispatch via its endpoint_map (l1..l16),
// so a report on src_endpoint 1 surfaces suffixed keys (e.g.
// `current_cooling_setpoint_l1`). The E-Ctrl has no endpoint_map, so its
// keys stay bare.
//
// z2m-source: zigbee-herdsman-converters/src/devices/imhotepcreation.ts
//             #E-Ctrl / #BRI4P + converters/fromZigbee.ts fz.thermostat
//             (occupiedCoolingSetpoint + min/max heat/cool setpoint-limit
//             branches).

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::imhotepcreation {
extern const PreparedDefinition kDef_E_Ctrl;
extern const PreparedDefinition kDef_BRI4P;
}  // namespace zhc::devices::imhotepcreation

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

// ── E-Ctrl: heater thermostat — min/max heat setpoint limits ─────────
void check_e_ctrl() {
    const auto& def = devices::imhotepcreation::kDef_E_Ctrl;

    // Exposes the climate surface + the two limits + occupancy.
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "min_heat_setpoint_limit"));
    assert(def_exposes(def, "max_heat_setpoint_limit"));
    assert(def_exposes(def, "occupancy"));

    // ── Parity fix: attr 0x0015 MinHeatSetpointLimit, s16 = 0x01F4 =
    //    500 → min_heat_setpoint_limit (raw, runtime /100 → 5.00 °C). ──
    const std::uint8_t lo[] = {0xF4, 0x01};
    auto rmin = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                             attr_report(0x0015, 0x29, lo));
    assert(rmin.any_matched);
    const Value* mn = rmin.merged.find("min_heat_setpoint_limit");
    assert(mn && mn->type == ValueType::Int && mn->i == 500);

    // ── attr 0x0016 MaxHeatSetpointLimit, s16 = 0x0BB8 = 3000 → 30 °C. ─
    const std::uint8_t hi[] = {0xB8, 0x0B};
    auto rmax = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                             attr_report(0x0016, 0x29, hi));
    assert(rmax.any_matched);
    const Value* mx = rmax.merged.find("max_heat_setpoint_limit");
    assert(mx && mx->type == ValueType::Int && mx->i == 3000);

    // ── Regression: core surface still decodes via generic kFzThermostat. ──
    const std::uint8_t lt[] = {0x34, 0x08};   // 0x0834 = 2100 → 21.00 °C raw
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    const std::uint8_t sp[] = {0xD0, 0x07};   // 0x07D0 = 2000
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0012, 0x29, sp));
    const Value* sv = rs.merged.find("current_heating_setpoint");
    assert(sv && sv->type == ValueType::Int && sv->i == 2000);

    const std::uint8_t sm[] = {0x04};         // system_mode = 4 (heat)
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 4);

    // ── Regression: occupancy via generic kFzOccupancy (attr 0x0000 bit0). ──
    const std::uint8_t occ_on[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0406, "msOccupancySensing", 1,
                           attr_report(0x0000, 0x18, occ_on));
    const Value* ov = ro.merged.find("occupancy");
    assert(ov && ov->type == ValueType::Bool && ov->b == true);
}

// ── BRI4P: 16-channel bridge — cooling setpoint + heat/cool limits ───
void check_bri4p() {
    const auto& def = devices::imhotepcreation::kDef_BRI4P;

    // Exposes (declared bare; dispatch suffixes per endpoint).
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "current_cooling_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "min_heat_setpoint_limit"));
    assert(def_exposes(def, "max_heat_setpoint_limit"));
    assert(def_exposes(def, "min_cool_setpoint_limit"));
    assert(def_exposes(def, "max_cool_setpoint_limit"));

    // BRI4P opts into per-endpoint dispatch → keys suffixed _l<n>.
    // Report on src_endpoint 1 → suffix _l1.

    // ── attr 0x0011 OccupiedCoolingSetpoint, s16 = 0x0898 = 2200 → 22 °C. ─
    const std::uint8_t cool[] = {0x98, 0x08};
    auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0011, 0x29, cool));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("current_cooling_setpoint_l1");
    assert(cv && cv->type == ValueType::Int && cv->i == 2200);

    // ── attr 0x0015 MinHeatSetpointLimit = 500, 0x0016 Max = 3000. ──
    const std::uint8_t hlo[] = {0xF4, 0x01};
    auto rh1 = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                            attr_report(0x0015, 0x29, hlo));
    const Value* hmn = rh1.merged.find("min_heat_setpoint_limit_l1");
    assert(hmn && hmn->type == ValueType::Int && hmn->i == 500);

    const std::uint8_t hhi[] = {0xB8, 0x0B};
    auto rh2 = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                            attr_report(0x0016, 0x29, hhi));
    const Value* hmx = rh2.merged.find("max_heat_setpoint_limit_l1");
    assert(hmx && hmx->type == ValueType::Int && hmx->i == 3000);

    // ── attr 0x0017 MinCoolSetpointLimit = 500, 0x0018 Max = 3800. ──
    const std::uint8_t clo[] = {0xF4, 0x01};
    auto rc1 = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                            attr_report(0x0017, 0x29, clo));
    const Value* cmn = rc1.merged.find("min_cool_setpoint_limit_l1");
    assert(cmn && cmn->type == ValueType::Int && cmn->i == 500);

    const std::uint8_t chi[] = {0xD8, 0x0E};  // 0x0ED8 = 3800
    auto rc2 = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                            attr_report(0x0018, 0x29, chi));
    const Value* cmx = rc2.merged.find("max_cool_setpoint_limit_l1");
    assert(cmx && cmx->type == ValueType::Int && cmx->i == 3800);

    // ── A report on endpoint 3 suffixes _l3 (per-channel routing). ──
    const std::uint8_t cool3[] = {0x10, 0x0E};  // 0x0E10 = 3600
    auto re3 = dispatch_zcl(def, 0x0201, "hvacThermostat", 3,
                            attr_report(0x0011, 0x29, cool3));
    const Value* cv3 = re3.merged.find("current_cooling_setpoint_l3");
    assert(cv3 && cv3->type == ValueType::Int && cv3->i == 3600);

    // ── Regression: core surface via generic kFzThermostat, suffixed. ──
    const std::uint8_t lt[] = {0x34, 0x08};   // 0x0834 = 2100
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature_l1");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    const std::uint8_t sm[] = {0x03};         // system_mode = 3 (cool)
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode_l1");
    assert(mv && mv->type == ValueType::Uint && mv->u == 3);
}

}  // namespace

int main() {
    check_e_ctrl();
    check_bri4p();
    return 0;
}
