// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the LUX KONOz KN-Z-WH1-B04 HVAC thermostat.
//
// z2m wires `fz.battery + fz.thermostat + fz.fan` and exposes a climate
// surface with occupied_heating_setpoint / occupied_cooling_setpoint /
// local_temperature / system_mode / running_state / fan_mode.
//
// Real gap: the auto-port wired the generic `kFzThermostat`, which
// decodes hvacThermostat (0x0201) attrs 0x0000 (local_temperature),
// 0x0012 (current_heating_setpoint) and 0x001C (system_mode) only. The
// device's `occupied_cooling_setpoint` (attr 0x0011) and `running_state`
// (attr 0x0029) — both decoded by z2m's `fz.thermostat` — were dead
// exposes. z2m's fz.thermostat also decodes 0x001E (running_mode). Fixed
// by wiring `kFzLuxThermostatExtras` + the matching exposes. The fan_mode
// (hvacFanCtrl 0x0202 attr 0x0000) was already decoded by the generic
// `kFzFanMode` and is regression-checked here.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lux.ts
//             #KN-Z-WH1-B04 + converters/fromZigbee.ts fz.thermostat /
//             fz.fan + lib/constants.ts thermostatRunningStates /
//             thermostatRunningMode.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lux {
extern const PreparedDefinition kDef_KN_Z_WH1_B04;
}  // namespace zhc::devices::lux

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

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then a record.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

void check_thermostat_extras() {
    const auto& def = devices::lux::kDef_KN_Z_WH1_B04;

    // Exposes declared (fixed: cooling setpoint + running state/mode).
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "fan_mode"));
    assert(def_exposes(def, "occupied_cooling_setpoint"));
    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "running_mode"));

    // ── attr 0x0011 OccupiedCoolingSetpoint, s16 (type 0x29).
    //    2500 = 0x09C4 → raw 2500 (downstream /100 = 25.00 °C). ──
    const std::uint8_t csp[] = {0xC4, 0x09};
    auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0011, 0x29, csp));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("occupied_cooling_setpoint");
    assert(cv && cv->type == ValueType::Int && cv->i == 2500);

    // ── attr 0x0029 ThermostatRunningState, bitmap16 (type 0x19). ──
    //   0x0001 → "heat".
    const std::uint8_t heat[] = {0x01, 0x00};
    auto rh = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0029, 0x19, heat));
    assert(rh.any_matched);
    const Value* hv = rh.merged.find("running_state");
    assert(hv && hv->type == ValueType::StringRef &&
           std::strcmp(hv->str, "heat") == 0);

    //   0x0002 → "cool".
    const std::uint8_t cool[] = {0x02, 0x00};
    auto rcool = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0029, 0x19, cool));
    const Value* coolv = rcool.merged.find("running_state");
    assert(coolv && std::strcmp(coolv->str, "cool") == 0);

    //   0x0000 → "idle".
    const std::uint8_t idle[] = {0x00, 0x00};
    auto ri = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0029, 0x19, idle));
    const Value* iv = ri.merged.find("running_state");
    assert(iv && std::strcmp(iv->str, "idle") == 0);

    //   Unknown running state (e.g. 0x0007) is dropped (lookup miss).
    const std::uint8_t bad[] = {0x07, 0x00};
    auto rb = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0029, 0x19, bad));
    assert(rb.merged.find("running_state") == nullptr);

    // ── attr 0x001E ThermostatRunningMode, enum8 (type 0x30). ──
    //   4 → "heat", 3 → "cool", 0 → "off".
    const std::uint8_t rm_heat[] = {0x04};
    auto rmh = dispatch_zcl(def, 0x0201, "hvacThermostat",
                            attr_report(0x001E, 0x30, rm_heat));
    const Value* rmhv = rmh.merged.find("running_mode");
    assert(rmhv && rmhv->type == ValueType::StringRef &&
           std::strcmp(rmhv->str, "heat") == 0);

    const std::uint8_t rm_off[] = {0x00};
    auto rmo = dispatch_zcl(def, 0x0201, "hvacThermostat",
                            attr_report(0x001E, 0x30, rm_off));
    const Value* rmov = rmo.merged.find("running_mode");
    assert(rmov && std::strcmp(rmov->str, "off") == 0);

    //   Unknown running mode (e.g. 7) is dropped (getFromLookup miss).
    const std::uint8_t rm_bad[] = {0x07};
    auto rmb = dispatch_zcl(def, 0x0201, "hvacThermostat",
                            attr_report(0x001E, 0x30, rm_bad));
    assert(rmb.merged.find("running_mode") == nullptr);

    // ── Regression: core surface still decodes via generic kFzThermostat ──
    // attr 0x0000 LocalTemperature, s16 = 0x0834 = 2100 → 21.00 °C raw.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // attr 0x0012 OccupiedHeatingSetpoint, s16 = 2000 → current_heating_setpoint.
    const std::uint8_t hsp[] = {0xD0, 0x07};
    auto rhs = dispatch_zcl(def, 0x0201, "hvacThermostat",
                            attr_report(0x0012, 0x29, hsp));
    const Value* hsv = rhs.merged.find("current_heating_setpoint");
    assert(hsv && hsv->type == ValueType::Int && hsv->i == 2000);

    // attr 0x001C SystemMode, u8 enum = 4 (heat) → raw u8.
    const std::uint8_t sm[] = {0x04};
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001C, 0x30, sm));
    const Value* sv = rs.merged.find("system_mode");
    assert(sv && sv->type == ValueType::Uint && sv->u == 4);
}

void check_fan_mode() {
    const auto& def = devices::lux::kDef_KN_Z_WH1_B04;

    // hvacFanCtrl (0x0202) attr 0x0000 FanMode, enum8 (type 0x30).
    // 5 → "auto" per constants.fanMode; generic kFzFanMode emits the
    // raw u8 enum under the "fan_mode" key.
    const std::uint8_t fm[] = {0x05};
    auto rf = dispatch_zcl(def, 0x0202, "hvacFanCtrl",
                           attr_report(0x0000, 0x30, fm));
    assert(rf.any_matched);
    const Value* fv = rf.merged.find("fan_mode");
    assert(fv && fv->type == ValueType::Uint && fv->u == 5);
}

}  // namespace

int main() {
    check_thermostat_extras();
    check_fan_mode();
    return 0;
}
