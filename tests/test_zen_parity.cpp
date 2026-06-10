// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Zen Zen-01-W thermostat. z2m wires the full
// `fz.thermostat` (entire hvacThermostat 0x0201 attribute surface) + `fz.fan`
// + `fz.battery`. The auto-generated port only wired the generic
// `kFzThermostat`, which decodes 0x0000 / 0x0012 / 0x001C and skips everything
// else — leaving three declared exposes dead:
//
//   * occupied_cooling_setpoint     (attr 0x0011, s16, raw 1/100 °C)
//   * local_temperature_calibration (attr 0x0010, s8)
//   * running_state                 (attr 0x0029, map16 enum, raw)
//
// Fixed by wiring the vendor `kFzZenThermostat` (definitions/zen/_shared.cpp)
// alongside the generic decoder + kFzFanMode. The core surface
// (local_temperature / current_heating_setpoint / system_mode) stays on the
// generic `kFzThermostat` and fan_mode on the generic `kFzFanMode`; all are
// regression-guarded here.
//
// z2m-source: zigbee-herdsman-converters/src/devices/zen.ts #Zen-01-W.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::zen {
extern const PreparedDefinition kDef_Zen_01_W;
}  // namespace zhc::devices::zen

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

// The Zen-01-W lives on endpoint 1 (z2m getEndpoint(3) || getEndpoint(1); the
// auto-port bound EP1). The converters use WILDCARD_ENDPOINT so the source
// endpoint is irrelevant to matching, but report against EP1 to mirror it.
constexpr std::uint8_t kEp = 1;

void check_zen01() {
    const auto& def = devices::zen::kDef_Zen_01_W;

    // ── Expose surface: the three "extra" exposes are declared (regression:
    // they were missing entirely from the auto-port and are now restored). ──
    assert(def_exposes(def, "occupied_cooling_setpoint"));
    assert(def_exposes(def, "local_temperature_calibration"));
    assert(def_exposes(def, "running_state"));
    // Core surface exposes.
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "fan_mode"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));

    // ── occupied_cooling_setpoint (0x0011, s16) = 0x09C4 = 2500 (25.00 °C) ──
    const std::uint8_t cool[] = {0xC4, 0x09};
    auto rc = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x0011, 0x29, cool));
    assert(rc.any_matched);
    const Value* cv = rc.merged.find("occupied_cooling_setpoint");
    assert(cv && cv->type == ValueType::Int && cv->i == 2500);

    // ── local_temperature_calibration (0x0010, s8) = -10 (0xF6) ──
    const std::uint8_t cal[] = {0xF6};
    auto rca = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                            attr_report(0x0010, 0x28, cal));
    const Value* lv = rca.merged.find("local_temperature_calibration");
    assert(lv && lv->type == ValueType::Int && lv->i == -10);

    // ── running_state (0x0029, map16) = 0x0002 = "cool" (raw enum forward) ──
    const std::uint8_t rs[] = {0x02, 0x00};
    auto rrs = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                            attr_report(0x0029, 0x19, rs));
    const Value* rv = rrs.merged.find("running_state");
    assert(rv && rv->type == ValueType::Uint && rv->u == 2);

    // running_state = 0 ("idle") must still be emitted, not swallowed as falsy.
    const std::uint8_t idle[] = {0x00, 0x00};
    auto rid = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                            attr_report(0x0029, 0x19, idle));
    const Value* iv = rid.merged.find("running_state");
    assert(iv && iv->type == ValueType::Uint && iv->u == 0);

    // ── Regression: core surface still decodes via generic kFzThermostat ──
    // 0x0000 LocalTemperature, s16 = 0x0834 = 2100.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // 0x0012 OccupiedHeatingSetpoint, s16 = 0x07D0 = 2000 → current_heating_setpoint.
    const std::uint8_t sp[] = {0xD0, 0x07};
    auto rsp = dispatch_zcl(def, 0x0201, "hvacThermostat", kEp,
                            attr_report(0x0012, 0x29, sp));
    const Value* sv = rsp.merged.find("current_heating_setpoint");
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
    check_zen01();
    return 0;
}
