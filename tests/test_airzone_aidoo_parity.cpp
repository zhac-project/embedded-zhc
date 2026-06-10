// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Airzone Aidoo AZAI6ZBEMHI AC controller. The
// auto-generated port declared (and originally even dropped) a cooling
// setpoint that the generic `kFzThermostat` — which decodes only
// hvacThermostat attrs 0x0000 / 0x0012 / 0x001C — never filled, so the
// cooling-setpoint surface was dead against the z2m wire shape:
//
//   * AZAI6ZBEMHI declares `current_cooling_setpoint`
//     (z2m `.withSetpoint("occupied_cooling_setpoint", ...)`), decoded
//     by `fz.thermostat` from attr 0x0011 (OccupiedCoolingSetpoint).
//     Fixed by wiring `kFzAirzoneThermostatExtras`.
//
// The core thermostat surface (local_temperature / current_heating_
// setpoint / system_mode) is still decoded by the generic kFzThermostat
// wired alongside, the switch state by kFzOnOff, and fan_mode by the
// generic kFzFanMode — all regression-checked here.
//
// z2m-source: zigbee-herdsman-converters/src/devices/airzone_aidoo.ts
//             #AZAI6ZBEMHI + converters/fromZigbee.ts fz.thermostat /
//             fz.on_off / fz.fan.

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

namespace zhc::devices::airzone_aidoo {
extern const PreparedDefinition kDef_AZAI6ZBEMHI;
}  // namespace zhc::devices::airzone_aidoo

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

ExposeType expose_type(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return def.exposes[i].type;
    return ExposeType::Binary;
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

// ── AZAI6ZBEMHI: cooling setpoint parity fix + core regressions ───────
void check_azai6zbemhi() {
    const auto& def = devices::airzone_aidoo::kDef_AZAI6ZBEMHI;

    // Expose surface: cooling setpoint present, modes are enums.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "current_cooling_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "fan_mode"));
    assert(expose_type(def, "system_mode") == ExposeType::Enum);
    assert(expose_type(def, "fan_mode") == ExposeType::Enum);

    // ── Parity fix: attr 0x0011 OccupiedCoolingSetpoint, s16 =
    //    0x0708 = 1800 → current_cooling_setpoint (raw, runtime /100). ──
    const std::uint8_t cool[] = {0x08, 0x07};
    auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0011, 0x29, cool));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("current_cooling_setpoint");
    assert(cv && cv->type == ValueType::Int && cv->i == 1800);

    // ── Regression: core surface still decodes via generic kFzThermostat ──
    const std::uint8_t lt[] = {0x34, 0x08};   // 0x0834 = 2100 → 21.00 °C raw
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    const std::uint8_t hsp[] = {0xD0, 0x07};  // 0x07D0 = 2000 heating setpoint
    auto rh = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x0012, 0x29, hsp));
    const Value* hv = rh.merged.find("current_heating_setpoint");
    assert(hv && hv->type == ValueType::Int && hv->i == 2000);

    const std::uint8_t sm[] = {0x03};         // system_mode = 3 (cool)
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 3);

    // ── Regression: fan_mode via generic kFzFanMode (hvacFanCtrl 0x0202,
    //    attr 0x0000 FanMode = 5 (auto)). z2m expose key is fan_mode. ──
    const std::uint8_t fm[] = {0x05};
    auto rf = dispatch_zcl(def, 0x0202, "hvacFanCtrl", 1,
                           attr_report(0x0000, 0x30, fm));
    assert(rf.any_matched);
    const Value* fv = rf.merged.find("fan_mode");
    assert(fv && fv->type == ValueType::Uint && fv->u == 5);

    // ── Regression: switch state via generic kFzOnOff (genOnOff 0x0006,
    //    attr 0x0000 OnOff bool). ──
    const std::uint8_t on[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                           attr_report(0x0000, 0x10, on));
    assert(ro.any_matched);
    const Value* ov = ro.merged.find("state");
    assert(ov && ov->type == ValueType::Bool && ov->b == true);
}

}  // namespace

int main() {
    check_azai6zbemhi();
    return 0;
}
