// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Leviton RC-2000WH (Omnistat2) thermostat. z2m
// wires the full `fz.thermostat`, which publishes the entire
// hvacThermostat (0x0201) attribute surface. The auto-generated port only
// wired the generic `kFzThermostat`, which decodes 0x0000 / 0x0012 /
// 0x001C and skips everything else — leaving three declared exposes dead:
//
//   * pi_heating_demand            (attr 0x0008, u8 → mapped 0-255→0-100 %)
//   * local_temperature_calibration (attr 0x0010, s8)
//   * occupied_cooling_setpoint     (attr 0x0011, s16, raw 1/100 °C)
//
// Fixed by wiring the vendor `kFzLevitonThermostat` (definitions/leviton/
// _shared.cpp) alongside the generic decoder + kFzFanMode. Unlike Danfoss,
// Leviton does NOT set `dontMapPIHeatingDemand`, so pi_heating_demand goes
// through `mapNumberRange(raw, 0, 255, 0, 100)` (round-half-up).
//
// The core surface (local_temperature / current_heating_setpoint /
// system_mode) is decoded by the generic `kFzThermostat` and fan_mode by
// the generic `kFzFanMode`; both are regression-guarded here.
//
// z2m-source: zigbee-herdsman-converters/src/devices/leviton.ts #RC-2000WH.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::leviton {
extern const PreparedDefinition kDef_RC_2000WH;
}  // namespace zhc::devices::leviton

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

// The RC-2000WH thermostat lives on endpoint 9 (z2m getEndpoint(9)); the
// converters use WILDCARD_ENDPOINT so the source endpoint is irrelevant to
// matching, but report against EP9 to mirror the real device.
constexpr std::uint8_t kEp = 9;

void check_rc2000() {
    const auto& def = devices::leviton::kDef_RC_2000WH;

    // All four "extra" exposes are declared (regression: they always were,
    // but never reached a value before the converter fix).
    assert(def_exposes(def, "pi_heating_demand"));
    assert(def_exposes(def, "occupied_cooling_setpoint"));
    assert(def_exposes(def, "local_temperature_calibration"));
    // Core surface exposes.
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "fan_mode"));

    // ── Core fix: pi_heating_demand (0x0008, u8 0-255 → 0-100 %) ──
    // raw 255 → 100 %.
    const std::uint8_t full[] = {0xFF};
    auto rf = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x0008, 0x20, full));
    assert(rf.any_matched);
    const Value* pf = rf.merged.find("pi_heating_demand");
    assert(pf && pf->type == ValueType::Uint && pf->u == 100);

    // raw 128 → round(50.196) = 50 %.
    const std::uint8_t half[] = {0x80};
    auto rh = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x0008, 0x20, half));
    const Value* ph = rh.merged.find("pi_heating_demand");
    assert(ph && ph->type == ValueType::Uint && ph->u == 50);

    // raw 0 → 0 % (must still be emitted, not swallowed as falsy).
    const std::uint8_t zero[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x0008, 0x20, zero));
    const Value* pz = rz.merged.find("pi_heating_demand");
    assert(pz && pz->type == ValueType::Uint && pz->u == 0);

    // ── occupied_cooling_setpoint (0x0011, s16) = 0x09C4 = 2500 (25.00 °C) ──
    const std::uint8_t cool[] = {0xC4, 0x09};
    auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x0011, 0x29, cool));
    const Value* cv = rc.merged.find("occupied_cooling_setpoint");
    assert(cv && cv->type == ValueType::Int && cv->i == 2500);

    // ── local_temperature_calibration (0x0010, s8) = -10 (0xF6) ──
    const std::uint8_t cal[] = {0xF6};
    auto rca = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                            attr_report(0x0010, 0x28, cal));
    const Value* lv = rca.merged.find("local_temperature_calibration");
    assert(lv && lv->type == ValueType::Int && lv->i == -10);

    // ── Regression: core surface still decodes via generic kFzThermostat ──
    // 0x0000 LocalTemperature, s16 = 0x0834 = 2100.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // 0x0012 OccupiedHeatingSetpoint, s16 = 0x07D0 = 2000 → current_heating_setpoint.
    const std::uint8_t sp[] = {0xD0, 0x07};
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x0012, 0x29, sp));
    const Value* sv = rs.merged.find("current_heating_setpoint");
    assert(sv && sv->type == ValueType::Int && sv->i == 2000);

    // 0x001C SystemMode, u8 enum = 4 (heat).
    const std::uint8_t sm[] = {0x04};
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 4);

    // ── fan_mode via generic kFzFanMode (hvacFanCtrl 0x0202 attr 0x0000) ──
    const std::uint8_t fan[] = {0x05};   // "auto" enum (constants.fanMode)
    auto rfa = dispatch_zcl(def, 0x0202, "hvacFanCtrl", kEp,
                            attr_report(0x0000, 0x30, fan));
    const Value* fv = rfa.merged.find("fan_mode");
    assert(fv && fv->type == ValueType::Uint && fv->u == 5);
}

}  // namespace

int main() {
    check_rc2000();
    return 0;
}
