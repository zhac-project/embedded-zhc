// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Plugwise heating family + plug.
//
// The auto-generated thermostat ports (Tom 106-03, Emma 170-01) declared
// several exposes that the generic `kFzThermostat` — which decodes only
// hvacThermostat attrs 0x0000 / 0x0012 / 0x001C — never filled. z2m's base
// `fz.thermostat` decodes them; without a converter they were DEAD:
//
//   * 106-03 Tom: `pi_heating_demand` (attr 0x0008 PIHeatingDemand, u8,
//     reported 0..100 on Plugwise). Fixed by extending
//     `kFzPlugwiseThermostat` to decode 0x0008.
//
//   * 170-01 Emma: `occupied_cooling_setpoint` (0x0011), `running_state`
//     (0x0029, map16 → string), `local_temperature_calibration` (0x0010)
//     and `outdoor_temperature` (0x0001) were all dead. Fixed by the same
//     extended converter. Emma also overloads pIHeatingDemand (0x0008) as
//     the boiler-water setpoint, surfaced by z2m as `boiler_setpoint` (°C),
//     wired here via the def-local `fz_d170_boiler_setpoint`.
//
// The core thermostat surface (local_temperature / current_heating_setpoint
// / system_mode) is still decoded by the generic kFzThermostat wired
// alongside, and is regression-checked here. The Lisa (158-01) and plug
// (160-01) are pure-generic and checked for regression too.
//
// z2m-source: zigbee-herdsman-converters/src/devices/plugwise.ts
//             #106-03 / #158-01 / #160-01 / #170-01 +
//             converters/fromZigbee.ts fz.thermostat / fz.metering +
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

namespace zhc::devices::plugwise {
extern const PreparedDefinition kDef_D106_03;
extern const PreparedDefinition kDef_D158_01;
extern const PreparedDefinition kDef_D160_01;
extern const PreparedDefinition kDef_D170_01;
}  // namespace zhc::devices::plugwise

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

// ── Plain ZCL attribute report: fc=0x18, tsn, cmd=0x0A, then records. ─
// Standard hvacThermostat attrs (0x0000/0x0008/0x0010/0x0011/0x0012/
// 0x001C/0x0029/0x0001) are reported without a manufacturer header,
// matching z2m's base fz.thermostat.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── Manufacturer-specific attribute report (Plugwise mfg 0x1172). ─────
// fc=0x1C (mfg-specific + server→client + disable-default), then mfg LE,
// tsn, cmd=0x0A, then records. Used for the 0x40xx Plugwise attrs.
std::vector<std::uint8_t> attr_report_mfg(std::uint16_t attr_id, std::uint8_t type,
                                          std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x1C, 0x72, 0x11, 0x42, 0x0A};  // mfg 0x1172
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool streq(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

// ── 106-03 Tom TRV: pi_heating_demand + manuSpec valve_position ───────
void check_d106_03() {
    const auto& def = devices::plugwise::kDef_D106_03;

    assert(def_exposes(def, "pi_heating_demand"));
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "valve_position"));
    assert(def_exposes(def, "battery"));

    // ── Parity fix: attr 0x0008 PIHeatingDemand, u8 = 55 → 55 % (identity;
    //    Plugwise reports 0..100 directly). Was dead before the converter
    //    fix — generic kFzThermostat skips 0x0008. ──
    const std::uint8_t demand[] = {0x37};   // 55
    auto rd = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, demand));
    assert(rd.any_matched);
    const Value* pd = rd.merged.find("pi_heating_demand");
    assert(pd && pd->type == ValueType::Uint && pd->u == 55);

    // Boundary: 0 % must still emit (not swallowed as falsy).
    const std::uint8_t zero[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, zero));
    const Value* pz = rz.merged.find("pi_heating_demand");
    assert(pz && pz->type == ValueType::Uint && pz->u == 0);

    // ── manuSpec attr 0x4001 plugwiseValvePosition, u8 = 80 → valve_position.
    const std::uint8_t vp[] = {0x50};   // 80
    auto rv = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report_mfg(0x4001, 0x20, vp));
    assert(rv.any_matched);
    const Value* vv = rv.merged.find("valve_position");
    assert(vv && vv->type == ValueType::Uint && vv->u == 80);

    // ── Regression: local_temperature decodes (generic kFzThermostat). ──
    const std::uint8_t lt[] = {0x34, 0x08};   // 0x0834 = 2100 → 21.00 °C raw
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);
}

// ── 170-01 Emma: cooling setpoint + running_state + calibration +
//    outdoor_temperature + boiler_setpoint + regressions ──────────────
void check_d170_01() {
    const auto& def = devices::plugwise::kDef_D170_01;

    assert(def_exposes(def, "occupied_cooling_setpoint"));
    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "local_temperature_calibration"));
    assert(def_exposes(def, "outdoor_temperature"));
    assert(def_exposes(def, "boiler_setpoint"));
    // z2m renamed pIHeatingDemand → boiler_setpoint on Emma; the TRV's
    // "pi_heating_demand" must NOT be exposed here.
    assert(!def_exposes(def, "pi_heating_demand"));
    assert(def_exposes(def, "system_mode"));

    // ── Parity fix 1: attr 0x0011 OccupiedCoolingSetpoint, s16 =
    //    0x0898 = 2200 → occupied_cooling_setpoint (raw, runtime /100). ──
    const std::uint8_t cool[] = {0x98, 0x08};
    auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0011, 0x29, cool));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("occupied_cooling_setpoint");
    assert(cv && cv->type == ValueType::Int && cv->i == 2200);

    // ── Parity fix 2: attr 0x0029 ThermostatRunningState, map16. ──
    //   2 → "cool"
    const std::uint8_t rs_cool[] = {0x02, 0x00};
    auto rr = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0029, 0x19, rs_cool));
    assert(rr.any_matched);
    assert(streq(rr.merged.find("running_state"), "cool"));
    //   1 → "heat"
    const std::uint8_t rs_heat[] = {0x01, 0x00};
    auto rh = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0029, 0x19, rs_heat));
    assert(streq(rh.merged.find("running_state"), "heat"));
    //   0 → "idle"
    const std::uint8_t rs_idle[] = {0x00, 0x00};
    auto ri = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0029, 0x19, rs_idle));
    assert(streq(ri.merged.find("running_state"), "idle"));

    // ── Parity fix 3: attr 0x0010 LocalTemperatureCalibration, s8 =
    //    0xFB = -5 → local_temperature_calibration (raw Int). ──
    const std::uint8_t cal[] = {0xFB};   // s8 -5
    auto rcal = dispatch_zcl(def, 0x0201, "hvacThermostat",
                             attr_report(0x0010, 0x28, cal));
    assert(rcal.any_matched);
    const Value* clv = rcal.merged.find("local_temperature_calibration");
    assert(clv && clv->type == ValueType::Int && clv->i == -5);

    // ── Parity fix 4: attr 0x0001 OutdoorTemperature, s16 =
    //    0x04B0 = 1200 → outdoor_temperature (raw, runtime /100). ──
    const std::uint8_t out[] = {0xB0, 0x04};
    auto ro = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0001, 0x29, out));
    assert(ro.any_matched);
    const Value* ov = ro.merged.find("outdoor_temperature");
    assert(ov && ov->type == ValueType::Int && ov->i == 1200);

    // ── Parity fix 5: attr 0x0008 PIHeatingDemand → boiler_setpoint (Emma
    //    overload, def-local converter). 60 → 60. ──
    const std::uint8_t bsp[] = {0x3C};   // 60
    auto rb = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, bsp));
    assert(rb.any_matched);
    const Value* bv = rb.merged.find("boiler_setpoint");
    assert(bv && bv->type == ValueType::Uint && bv->u == 60);

    // ── Regression: system_mode + heating setpoint via generic kFzThermostat ──
    const std::uint8_t sm[] = {0x04};   // heat
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 4);

    const std::uint8_t sp[] = {0xD0, 0x07};   // 0x07D0 = 2000
    auto rsp = dispatch_zcl(def, 0x0201, "hvacThermostat",
                            attr_report(0x0012, 0x29, sp));
    const Value* sv = rsp.merged.find("current_heating_setpoint");
    assert(sv && sv->type == ValueType::Int && sv->i == 2000);

    // ── Regression: manuSpec boiler_water_temperature (0xF002 INT16). ──
    const std::uint8_t bwt[] = {0xA4, 0x0E};   // 0x0EA4 = 3748 → 37.48 °C raw
    auto rbw = dispatch_zcl(def, 0x0201, "hvacThermostat",
                            attr_report_mfg(0xF002, 0x29, bwt));
    const Value* bwv = rbw.merged.find("boiler_water_temperature");
    assert(bwv && bwv->type == ValueType::Int && bwv->i == 3748);
}

// ── 158-01 Lisa: pure generic thermostat (regression only) ────────────
void check_d158_01() {
    const auto& def = devices::plugwise::kDef_D158_01;

    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "occupied_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "battery"));
    // Lisa has no demand/valve surface.
    assert(!def_exposes(def, "pi_heating_demand"));
    assert(!def_exposes(def, "valve_position"));

    const std::uint8_t lt[] = {0x34, 0x08};   // 2100
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    const std::uint8_t sm[] = {0x01};   // auto
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 1);
}

// ── 160-01 plug: on/off + metering (regression) ───────────────────────
void check_d160_01() {
    const auto& def = devices::plugwise::kDef_D160_01;

    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));

    // genOnOff attr 0x0000 OnOff (bool) = 1 → state "ON" (generic kFzOnOff).
    const std::uint8_t on[] = {0x01};
    auto rs = dispatch_zcl(def, 0x0006, "genOnOff",
                           attr_report(0x0000, 0x10, on));
    assert(rs.any_matched);
    const Value* sv = rs.merged.find("state");
    assert(sv != nullptr);

    // seMetering attr 0x0400 InstantaneousDemand, s24 = 0x0000C8 = 200 → power.
    const std::uint8_t demand[] = {0xC8, 0x00, 0x00};
    auto rp = dispatch_zcl(def, 0x0702, "seMetering",
                           attr_report(0x0400, 0x2A, demand));
    assert(rp.any_matched);
    const Value* pv = rp.merged.find("power");
    assert(pv && pv->type == ValueType::Int && pv->i == 200);
}

}  // namespace

int main() {
    check_d106_03();
    check_d170_01();
    check_d158_01();
    check_d160_01();
    return 0;
}
