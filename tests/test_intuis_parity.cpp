// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Intuis (Muller Intuitiv) intuisradiator electric
// heater. Pins the gaps fixed when the def was graduated out of
// generated/:
//
//   * The generic kFzThermostat only decodes hvacThermostat 0x0000 /
//     0x0012 / 0x001C and emits attr 0x0012 under "current_heating_
//     setpoint". The z2m climate expose declares "occupied_heating_
//     setpoint", so the occupied setpoint was a DEAD key; the
//     unoccupied setpoint (0x0014) and running_state (0x0029) were not
//     decoded at all. Now routed through kFzIntuisThermostat.
//   * occupancy (msOccupancySensing 0x0406) had no decoder wired — now
//     via the generic kFzOccupancy.
//   * keypad_lockout / temperature_display_mode (hvacUserInterfaceCfg
//     0x0204) had no decoder — now via kFzIntuisHvacUi.
//
// The def carries an endpoint_map {"1",1} (z2m meta.multiEndpoint +
// .withEndpoint("1")), so the dispatch layer suffixes the EP1 runtime
// keys to "_<label>" = "_1". Exposes stay bare; the asserts below use
// the suffixed runtime keys.
//
// z2m-source: zigbee-herdsman-converters/src/devices/intuis.ts +
//             converters/fromZigbee.ts #thermostat / #hvac_user_interface.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::intuis {
extern const PreparedDefinition kDef_intuisradiator;
}  // namespace zhc::devices::intuis

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
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
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

bool int_is(const Value* v, std::int64_t i) {
    return v && v->type == ValueType::Int && v->i == i;
}

}  // namespace

int main() {
    using namespace zhc::devices::intuis;
    const PreparedDefinition& def = kDef_intuisradiator;

    // ── Exposes: bare keys present, phantom power dropped ────────────
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "occupied_heating_setpoint"));
    assert(def_exposes(def, "unoccupied_heating_setpoint"));
    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "keypad_lockout"));
    assert(def_exposes(def, "temperature_display_mode"));
    // z2m sets electricityMeter power:false → no power expose, and the
    // old generic-decoder dead key must be gone.
    assert(!def_exposes(def, "power"));
    assert(!def_exposes(def, "current_heating_setpoint"));

    // local_temperature (0x0000 s16) = 2100 (21.00 °C raw), EP1 → _1.
    {
        const std::uint8_t b[] = {0x34, 0x08};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x0000, 0x29, b));
        assert(r.any_matched);
        assert(int_is(r.merged.find("local_temperature_1"), 2100));
    }
    // occupied_heating_setpoint (0x0012 s16) = 2000 — NOT
    // current_heating_setpoint (the generic-decoder dead key).
    {
        const std::uint8_t b[] = {0xD0, 0x07};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x0012, 0x29, b));
        assert(r.any_matched);
        assert(int_is(r.merged.find("occupied_heating_setpoint_1"), 2000));
        assert(r.merged.find("current_heating_setpoint_1") == nullptr);
        assert(r.merged.find("current_heating_setpoint") == nullptr);
    }
    // unoccupied_heating_setpoint (0x0014 s16) = 1600.
    {
        const std::uint8_t b[] = {0x40, 0x06};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x0014, 0x29, b));
        assert(r.any_matched);
        assert(int_is(r.merged.find("unoccupied_heating_setpoint_1"), 1600));
    }
    // running_state (0x0029) 1 → heat, 0 → idle.
    {
        const std::uint8_t heat[] = {0x01};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x0029, 0x30, heat));
        assert(r.any_matched);
        assert(str_is(r.merged.find("running_state_1"), "heat"));

        const std::uint8_t idle[] = {0x00};
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                               attr_report(0x0029, 0x30, idle));
        assert(str_is(r2.merged.find("running_state_1"), "idle"));
    }
    // system_mode (0x001C u8 enum) raw passthrough still works.
    {
        const std::uint8_t sm[] = {0x04};  // heat
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x001C, 0x30, sm));
        assert(r.any_matched);
        assert(r.merged.find("system_mode_1") != nullptr);
    }

    // ── occupancy (msOccupancySensing 0x0406 attr 0x0000) on EP1 ─────
    {
        const std::uint8_t occ[] = {0x01};
        auto r = dispatch_zcl(def, 0x0406, "msOccupancySensing", 1,
                              attr_report(0x0000, 0x18, occ));
        assert(r.any_matched);
        const Value* v = r.merged.find("occupancy_1");
        assert(v && v->type == ValueType::Bool && v->b == true);
    }

    // ── hvacUserInterfaceCfg (0x0204) keypad_lockout + display mode ──
    {
        // keypad_lockout (0x0000) 1 → lock1.
        const std::uint8_t kl[] = {0x01};
        auto r = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg", 1,
                              attr_report(0x0000, 0x30, kl));
        assert(r.any_matched);
        assert(str_is(r.merged.find("keypad_lockout_1"), "lock1"));

        // keypad_lockout 0 → unlock.
        const std::uint8_t ul[] = {0x00};
        auto r2 = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg", 1,
                               attr_report(0x0000, 0x30, ul));
        assert(str_is(r2.merged.find("keypad_lockout_1"), "unlock"));
    }
    {
        // temperature_display_mode (0x0001) 1 → fahrenheit.
        const std::uint8_t f[] = {0x01};
        auto r = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg", 1,
                              attr_report(0x0001, 0x30, f));
        assert(r.any_matched);
        assert(str_is(r.merged.find("temperature_display_mode_1"), "fahrenheit"));

        // 0 → celsius.
        const std::uint8_t c[] = {0x00};
        auto r2 = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg", 1,
                               attr_report(0x0001, 0x30, c));
        assert(str_is(r2.merged.find("temperature_display_mode_1"), "celsius"));
    }

    // ── energy (seMetering 0x0702 attr 0x0000) decodes and suffixes ──
    {
        const std::uint8_t e[] = {0x10, 0x27, 0x00, 0x00, 0x00, 0x00};  // 10000, u48
        auto r = dispatch_zcl(def, 0x0702, "seMetering", 1,
                              attr_report(0x0000, 0x25, e));
        assert(r.any_matched);
        assert(r.merged.find("energy_1") != nullptr);
    }

    return 0;
}
