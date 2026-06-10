// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Netica FRZ1 ("FreezBee") thermostat for Frisquet
// boilers. Pins the gaps fixed when the def was graduated out of
// generated/:
//
//   * PHANTOM battery removed. z2m's netica.ts wires no m.battery() and
//     no genPowerCfg, yet the earlier port carried battery/voltage
//     exposes + kFzBattery + a 0x0001 binding. Asserted absent here.
//   * running_state (hvacThermostat 0x0029) now decodes. z2m
//     m.thermostat({runningState}) exposes it and fz.thermostat decodes
//     it, but the generic kFzThermostat only handles 0x0000/0x0012/
//     0x001C — so it was missing. Decoded via kFzNeticaThermostatExtras
//     (0 → idle, 1 → heat).
//   * control_sequence_of_operation (0x001B) now decodes (heating_only).
//   * remote_temperature (manuSpec 0x4000) + target_water_temperature
//     (manuSpec 0x4002) now read back. They were write-only / dead
//     STATE_GET before.
//
// The standard surface (local_temperature, current_heating_setpoint,
// system_mode) still flows through the generic thermostat decoder.
//
// z2m-source: zigbee-herdsman-converters/src/devices/netica.ts #FRZ1.

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

namespace zhc::devices::netica {
extern const PreparedDefinition kDef_FRZ1;
}  // namespace zhc::devices::netica

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

bool str_is(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

}  // namespace

int main() {
    const PreparedDefinition& def = devices::netica::kDef_FRZ1;

    // ── Exposes surface ──────────────────────────────────────────────
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "control_sequence_of_operation"));
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "remote_temperature"));
    assert(def_exposes(def, "use_remote_temperature"));
    assert(def_exposes(def, "target_water_temperature"));

    // PHANTOM battery must be gone.
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));

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
    // SystemMode (0x001C u8 enum) raw passthrough.
    {
        const std::uint8_t sm[] = {0x04};  // heat
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x001C, 0x30, sm));
        assert(r.any_matched);
        assert(r.merged.find("system_mode") != nullptr);
    }

    // ── running_state (0x0029) ───────────────────────────────────────
    {
        const std::uint8_t heat[] = {0x01};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x0029, 0x30, heat));
        assert(r.any_matched);
        assert(str_is(r.merged.find("running_state"), "heat"));

        const std::uint8_t idle[] = {0x00};
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat",
                               attr_report(0x0029, 0x30, idle));
        assert(str_is(r2.merged.find("running_state"), "idle"));
    }

    // ── control_sequence_of_operation (0x001B) ───────────────────────
    {
        const std::uint8_t heating_only[] = {0x02};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x001B, 0x30, heating_only));
        assert(r.any_matched);
        assert(str_is(r.merged.find("control_sequence_of_operation"), "heating_only"));
    }

    // ── remote_temperature (manuSpec 0x4000, INT16 ×100) read-back ───
    {
        // 2345 = 23.45 °C raw. 0x0929 LE.
        const std::uint8_t rt[] = {0x29, 0x09};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x4000, 0x29, rt));
        assert(r.any_matched);
        const Value* v = r.merged.find("remote_temperature");
        assert(v && v->type == ValueType::Int && v->i == 2345);
    }

    // ── target_water_temperature (manuSpec 0x4002, INT16 ×100) ───────
    {
        // 4500 = 45.00 °C raw. 0x1194 LE.
        const std::uint8_t tw[] = {0x94, 0x11};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x4002, 0x29, tw));
        assert(r.any_matched);
        const Value* v = r.merged.find("target_water_temperature");
        assert(v && v->type == ValueType::Int && v->i == 4500);
    }

    // ── Temperature / humidity sensors still decode ──────────────────
    {
        // msTemperatureMeasurement 0x0402 attr 0x0000 (s16) = 2200.
        const std::uint8_t t[] = {0x98, 0x08};
        auto r = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement",
                              attr_report(0x0000, 0x29, t));
        assert(r.any_matched);
        assert(r.merged.find("temperature") != nullptr);

        // msRelativeHumidity 0x0405 attr 0x0000 (u16) = 5500.
        const std::uint8_t h[] = {0x7C, 0x15};
        auto r2 = dispatch_zcl(def, 0x0405, "msRelativeHumidity",
                               attr_report(0x0000, 0x21, h));
        assert(r2.any_matched);
        assert(r2.merged.find("humidity") != nullptr);
    }

    // ── Write path: remote_temperature → writeAttributes 0x4000 ──────
    {
        RuntimeContext ctx{};
        std::array<std::uint8_t, 24> frame{};
        Value v{}; v.type = ValueType::Int; v.i = 2000;
        auto w = dispatch_to_zigbee(def, "remote_temperature", v, ctx, frame);
        assert(w.ok);
        assert(w.cluster_id == 0x0201);
        assert(w.command_id == 0x02);   // writeAttributes
    }

    return 0;
}
