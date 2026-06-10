// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the eCozy 1TST-EU smart heating thermostat. Pins the
// gaps fixed when the def was graduated out of generated/:
//
//   * pi_heating_demand (hvacThermostat 0x0008), local_temperature_
//     calibration (0x0010) and running_state (0x0029) now decode. z2m's
//     ecozy.ts wires fz.thermostat (which decodes all three) and the
//     climate block exposes them, but the generic kFzThermostat only
//     handles 0x0000/0x0012/0x001C — so the three keys were exposed-but-
//     DEAD. Decoded via kFzEcozyThermostatExtras.
//   * Exposes upgraded from the impoverished auto block (Binary
//     system_mode, no running_state / lt_cal / pi_heating_demand) to the
//     real climate-flat surface z2m declares (Enum system_mode + the three
//     extra keys).
//
// The standard surface (local_temperature, current_heating_setpoint,
// system_mode) still flows through the generic thermostat decoder, and
// battery still decodes via the generic battery converter.
//
// z2m-source: zigbee-herdsman-converters/src/devices/ecozy.ts #1TST-EU.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ecozy {
extern const PreparedDefinition kDef_D1TST_EU;
}  // namespace zhc::devices::ecozy

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
    return ExposeType::Numeric;
}

bool str_is(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

}  // namespace

int main() {
    const PreparedDefinition& def = devices::ecozy::kDef_D1TST_EU;

    // ── Exposes surface ──────────────────────────────────────────────
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "local_temperature_calibration"));
    assert(def_exposes(def, "pi_heating_demand"));
    // system_mode is a real enum {off,auto,heat}, not the old Binary stub.
    assert(expose_type(def, "system_mode") == ExposeType::Enum);
    assert(expose_type(def, "running_state") == ExposeType::Enum);

    // ── Standard thermostat surface (generic decoder still flows) ────
    // LocalTemperature (0x0000 s16) = 0x0834 = 2100 (21.00 °C raw).
    {
        const std::uint8_t lt[] = {0x34, 0x08};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0000, 0x29, lt));
        assert(r.any_matched);
        const Value* v = r.merged.find("local_temperature");
        assert(v && v->type == ValueType::Int && v->i == 2100);
    }
    // OccupiedHeatingSetpoint (0x0012 s16) = 2200 → current_heating_setpoint.
    {
        const std::uint8_t sp[] = {0x98, 0x08};  // 2200
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0012, 0x29, sp));
        assert(r.any_matched);
        const Value* v = r.merged.find("current_heating_setpoint");
        assert(v && v->type == ValueType::Int && v->i == 2200);
    }
    // SystemMode (0x001C u8 enum) raw passthrough.
    {
        const std::uint8_t sm[] = {0x04};  // heat
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x001C, 0x30, sm));
        assert(r.any_matched);
        assert(r.merged.find("system_mode") != nullptr);
    }

    // ── pi_heating_demand (0x0008, raw 0-255 → 0-100 %) ──────────────
    {
        const std::uint8_t full[] = {0xFF};  // 255 → 100 %
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0008, 0x20, full));
        assert(r.any_matched);
        const Value* v = r.merged.find("pi_heating_demand");
        assert(v && v->type == ValueType::Uint && v->u == 100);

        const std::uint8_t half[] = {0x80};  // 128 → round((128*100)/255)=50
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat",
                               attr_report(0x0008, 0x20, half));
        const Value* v2 = r2.merged.find("pi_heating_demand");
        assert(v2 && v2->type == ValueType::Uint && v2->u == 50);
    }

    // ── local_temperature_calibration (0x0010, signed int8, raw/10 °C) ─
    {
        // raw = 0x05 = 5 → +0.5 °C.
        const std::uint8_t pos[] = {0x05};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0010, 0x28 /* INT8 */, pos));
        assert(r.any_matched);
        const Value* v = r.merged.find("local_temperature_calibration");
        assert(v && v->type == ValueType::Float && std::fabs(v->f - 0.5f) < 0.001f);

        // raw = 0xFB = -5 → -0.5 °C.
        const std::uint8_t neg[] = {0xFB};
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat",
                               attr_report(0x0010, 0x28, neg));
        const Value* v2 = r2.merged.find("local_temperature_calibration");
        assert(v2 && v2->type == ValueType::Float && std::fabs(v2->f + 0.5f) < 0.001f);
    }

    // ── running_state (0x0029, BITMAP16 → 2 bytes LE) ────────────────
    {
        const std::uint8_t heat[] = {0x01, 0x00};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0029, 0x19 /* BITMAP16 */, heat));
        assert(r.any_matched);
        assert(str_is(r.merged.find("running_state"), "heat"));

        const std::uint8_t idle[] = {0x00, 0x00};
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat",
                               attr_report(0x0029, 0x19, idle));
        assert(str_is(r2.merged.find("running_state"), "idle"));
    }

    // ── battery still decodes (genPowerCfg 0x0021, percent/2) ────────
    {
        const std::uint8_t pct[] = {0xC8};  // 200 → 100 %
        auto r = dispatch_zcl(def, 0x0001, "genPowerCfg",
                              attr_report(0x0021, 0x20, pct));
        assert(r.any_matched);
        const Value* v = r.merged.find("battery");
        assert(v && v->type == ValueType::Uint && v->u == 100);
    }

    return 0;
}
