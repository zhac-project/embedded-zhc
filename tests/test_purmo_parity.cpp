// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Purmo/Radson Yali Parada Plus (PUMM01102) electric
// oil-filled radiator thermostat. Pins the gaps fixed when the def was
// graduated out of generated/:
//
//   * The Tier-1 auto-port wired only the generic kFzThermostat, which
//     decodes ONLY hvacThermostat 0x0000 / 0x0012 / 0x001C. z2m's
//     fz.thermostat decodes the whole surface, so these exposed-but-dead
//     attrs are now decoded by kFzPurmoThermostatExtras:
//       0x0002 occupancy                    → occupancy (bit0 bool)
//       0x0010 localTemperatureCalibration  → local_temperature_calibration
//       0x0014 unoccupiedHeatingSetpoint    → unoccupied_heating_setpoint
//       0x0016 maxHeatSetpointLimit         → max_heat_setpoint_limit
//       0x001E runningMode                  → running_mode  (off/cool/heat)
//       0x0029 runningState                 → running_state (idle/heat)
//   * system_mode re-typed from Binary to Enum (z2m heat/off).
//
// local_temperature, current_heating_setpoint and system_mode still flow
// through the generic thermostat decoder.
//
// z2m-source: zigbee-herdsman-converters/src/devices/purmo.ts #Yali Parada Plus.

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

namespace zhc::devices::purmo {
extern const PreparedDefinition kDef_Yali_Parada_Plus;
}  // namespace zhc::devices::purmo

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

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then one record.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
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
    return ExposeType::Numeric;  // unreached for asserted keys
}

bool str_is(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

}  // namespace

int main() {
    const PreparedDefinition& def = devices::purmo::kDef_Yali_Parada_Plus;

    // ── Exposes surface ──────────────────────────────────────────────
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "unoccupied_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "local_temperature_calibration"));
    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "running_mode"));
    assert(def_exposes(def, "max_heat_setpoint_limit"));

    // system_mode must be an Enum now (was Binary in the Tier-1 port).
    assert(expose_type(def, "system_mode") == ExposeType::Enum);
    assert(expose_type(def, "running_state") == ExposeType::Enum);
    assert(expose_type(def, "running_mode") == ExposeType::Enum);

    // No phantom light/on-off surface on a thermostat.
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "state"));

    // ── Standard thermostat surface (generic decoder) ────────────────
    // LocalTemperature (0x0000 s16) = 0x0834 = 2100 (21.00 °C raw).
    {
        const std::uint8_t lt[] = {0x34, 0x08};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0000, 0x29, lt));
        assert(r.any_matched);
        const Value* v = r.merged.find("local_temperature");
        assert(v && v->type == ValueType::Int && v->i == 2100);
    }
    // OccupiedHeatingSetpoint (0x0012 s16) → current_heating_setpoint.
    {
        const std::uint8_t sp[] = {0xC4, 0x09};  // 2500
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0012, 0x29, sp));
        assert(r.any_matched);
        const Value* v = r.merged.find("current_heating_setpoint");
        assert(v && v->type == ValueType::Int && v->i == 2500);
    }
    // SystemMode (0x001C u8 enum) raw passthrough (generic).
    {
        const std::uint8_t sm[] = {0x04};  // heat
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x001C, 0x30, sm));
        assert(r.any_matched);
        assert(r.merged.find("system_mode") != nullptr);
    }

    // ── unoccupied_heating_setpoint (0x0014 s16) — was dead ──────────
    {
        const std::uint8_t sp[] = {0x84, 0x03};  // 900 = 9.00 °C raw
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0014, 0x29, sp));
        assert(r.any_matched);
        const Value* v = r.merged.find("unoccupied_heating_setpoint");
        assert(v && v->type == ValueType::Int && v->i == 900);
    }

    // ── local_temperature_calibration (0x0010 s8, ×10) — was dead ────
    {
        const std::uint8_t cal[] = {0xFB};  // -5 (×10 = -0.5 °C)
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0010, 0x28, cal));
        assert(r.any_matched);
        const Value* v = r.merged.find("local_temperature_calibration");
        assert(v && v->type == ValueType::Int && v->i == -5);
    }

    // ── max_heat_setpoint_limit (0x0016 s16) — was dead ──────────────
    {
        const std::uint8_t lim[] = {0xB8, 0x0B};  // 3000 = 30.00 °C raw
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0016, 0x29, lim));
        assert(r.any_matched);
        const Value* v = r.merged.find("max_heat_setpoint_limit");
        assert(v && v->type == ValueType::Int && v->i == 3000);
    }

    // ── running_mode (0x001E u8 enum) — was dead ─────────────────────
    {
        const std::uint8_t heat[] = {0x04};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x001E, 0x30, heat));
        assert(r.any_matched);
        assert(str_is(r.merged.find("running_mode"), "heat"));

        const std::uint8_t off[] = {0x00};
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat",
                               attr_report(0x001E, 0x30, off));
        assert(str_is(r2.merged.find("running_mode"), "off"));
    }

    // ── running_state (0x0029 bitmap16) — was dead ───────────────────
    {
        const std::uint8_t heat[] = {0x01, 0x00};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0029, 0x19, heat));
        assert(r.any_matched);
        assert(str_is(r.merged.find("running_state"), "heat"));

        const std::uint8_t idle[] = {0x00, 0x00};
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat",
                               attr_report(0x0029, 0x19, idle));
        assert(str_is(r2.merged.find("running_state"), "idle"));
    }

    // ── occupancy (0x0002 bitmap8, bit0) — was dead ──────────────────
    {
        const std::uint8_t occ[] = {0x01};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0002, 0x18, occ));
        assert(r.any_matched);
        const Value* v = r.merged.find("occupancy");
        assert(v && v->type == ValueType::Bool && v->b == true);

        const std::uint8_t unocc[] = {0x00};
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat",
                               attr_report(0x0002, 0x18, unocc));
        const Value* v2 = r2.merged.find("occupancy");
        assert(v2 && v2->type == ValueType::Bool && v2->b == false);
    }

    // ── Write path: unoccupied_heating_setpoint → writeAttributes ────
    {
        RuntimeContext ctx{};
        std::array<std::uint8_t, 24> frame{};
        Value v{}; v.type = ValueType::Int; v.i = 1000;
        auto w = dispatch_to_zigbee(def, "unoccupied_heating_setpoint", v, ctx, frame);
        assert(w.ok);
        assert(w.cluster_id == 0x0201);
        assert(w.command_id == 0x02);  // writeAttributes
    }
    // ── Write path: local_temperature_calibration → writeAttributes ──
    {
        RuntimeContext ctx{};
        std::array<std::uint8_t, 24> frame{};
        Value v{}; v.type = ValueType::Int; v.i = -5;
        auto w = dispatch_to_zigbee(def, "local_temperature_calibration", v, ctx, frame);
        assert(w.ok);
        assert(w.cluster_id == 0x0201);
        assert(w.command_id == 0x02);
    }

    return 0;
}
