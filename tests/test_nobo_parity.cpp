// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Nobø (Glen Dimplex) electric-heater thermostats
// SWT-IZ (floor heating) and SPC-IZ (panel-heater control unit). Both
// use z2m m.thermostat() and wire the single fz.thermostat converter.
//
// The auto-generated stub mis-ported these as a bare on/off + battery
// class (kFzBattery + kFzOnOff, exposing state/battery/voltage) — the
// whole thermostat surface was dead and every key was phantom. The
// graduated def now decodes via kFzNoboThermostat, emitting z2m's
// climate keys:
//   0x0000 → local_temperature
//   0x0012 → occupied_heating_setpoint  (NOT the generic dead key
//            current_heating_setpoint)
//   0x0014 → unoccupied_heating_setpoint
//   0x001C → system_mode  (raw enum passthrough)
//   0x001E → running_mode (string off/heat)
//
// The defs carry NO endpoint_map (z2m has no deviceEndpoints), so the
// dispatch layer leaves runtime keys BARE.
//
// z2m-source: zigbee-herdsman-converters/src/devices/nobo.ts +
//             converters/fromZigbee.ts #thermostat.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::nobo {
extern const PreparedDefinition kDef_SWT_IZ;
extern const PreparedDefinition kDef_SPC_IZ;
}  // namespace zhc::devices::nobo

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

// Exercise the full thermostat surface for one Nobø def.
void check_thermostat(const PreparedDefinition& def) {
    // ── Exposes: real thermostat class, phantom on/off + battery gone ──
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "occupied_heating_setpoint"));
    assert(def_exposes(def, "unoccupied_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "running_mode"));
    // The mis-ported stub keys must be gone, as must the generic dead key.
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));
    assert(!def_exposes(def, "current_heating_setpoint"));

    // local_temperature (0x0000 s16) = 2100 (21.00 °C raw), bare key.
    {
        const std::uint8_t b[] = {0x34, 0x08};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x0000, 0x29, b));
        assert(r.any_matched);
        assert(int_is(r.merged.find("local_temperature"), 2100));
    }
    // occupied_heating_setpoint (0x0012 s16) = 2000 — NOT the generic
    // dead key current_heating_setpoint.
    {
        const std::uint8_t b[] = {0xD0, 0x07};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x0012, 0x29, b));
        assert(r.any_matched);
        assert(int_is(r.merged.find("occupied_heating_setpoint"), 2000));
        assert(r.merged.find("current_heating_setpoint") == nullptr);
    }
    // unoccupied_heating_setpoint (0x0014 s16) = 1600.
    {
        const std::uint8_t b[] = {0x40, 0x06};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x0014, 0x29, b));
        assert(r.any_matched);
        assert(int_is(r.merged.find("unoccupied_heating_setpoint"), 1600));
    }
    // system_mode (0x001C u8 enum) raw passthrough.
    {
        const std::uint8_t sm[] = {0x04};  // heat
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x001C, 0x30, sm));
        assert(r.any_matched);
        const Value* v = r.merged.find("system_mode");
        assert(v && v->type == ValueType::Uint && v->u == 4);
    }
    // running_mode (0x001E enum8) 4 → heat, 0 → off.
    {
        const std::uint8_t heat[] = {0x04};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                              attr_report(0x001E, 0x30, heat));
        assert(r.any_matched);
        assert(str_is(r.merged.find("running_mode"), "heat"));

        const std::uint8_t off[] = {0x00};
        auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                               attr_report(0x001E, 0x30, off));
        assert(str_is(r2.merged.find("running_mode"), "off"));
    }
}

}  // namespace

int main() {
    using namespace zhc::devices::nobo;
    check_thermostat(kDef_SWT_IZ);
    check_thermostat(kDef_SPC_IZ);
    return 0;
}
