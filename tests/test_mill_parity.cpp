// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Mill (Norwegian electric heating) family. Both ports
// carried the same wrong-class bug that this fixture pins after the fix:
//
//   Wrong class — both Mill-gen-4 (WiFi heating panel) and MFTWIFI (smart floor
//   thermostat) are z2m m.thermostat() devices (hvacThermostat 0x0201:
//   occupiedHeatingSetpoint 5..35 + systemMode ["off","heat"]). The auto-port
//   mis-modelled each as a bare genOnOff switch + battery (phantom
//   state/battery/voltage, bind 0x0001/0x0006, no write path). Re-wired to the
//   generic kFzThermostat read + kTzThermostat write, bound 0x0201, and dropped
//   the phantom on/off + battery channels (the device has neither cluster).
//
//   Decoders pinned: local_temperature (attr 0x0000), current_heating_setpoint
//   (0x0012), system_mode (0x001C). Phantom "state"/"battery"/"voltage" gone.

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

namespace zhc::devices::mill {
extern const PreparedDefinition kDef_Mill_gen_4;  // WiFi heating panel gen4
extern const PreparedDefinition kDef_MFTWIFI;     // smart floor thermostat
}  // namespace zhc::devices::mill

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── Shape: a flat thermostat — no phantom on/off / battery, write path wired. ──
void check_thermostat_shape(const PreparedDefinition& def) {
    // Real thermostat channels exposed.
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    // Phantom on/off + battery channels gone.
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));
    // Settable: z2m m.thermostat() exposes a write path (tz.thermostat_*).
    assert(def.to_zigbee_count > 0);
    // Bound to hvacThermostat, not genOnOff/genPowerCfg.
    assert(def_binds(def, 1, 0x0201));
    assert(!def_binds(def, 1, 0x0006));
    assert(!def_binds(def, 1, 0x0001));
}

// ── Decoders: 0x0000/0x0012/0x001C on hvacThermostat all light up. ────────────
void check_thermostat_decode(const PreparedDefinition& def) {
    // LocalTemperature (0x0000, s16) 21.50 °C -> 2150 raw.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> t = {0x66, 0x08};  // 2150 LE
    auto tr = dispatch_zcl_ep(c1, def, 0x0201, 1, "hvacThermostat",
                              attr_report(0x0000, 0x29, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    assert(tr.merged.find("local_temperature") != nullptr);

    // OccupiedHeatingSetpoint (0x0012, s16) 22.00 °C -> 2200 raw -> current_heating_setpoint.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> s = {0x98, 0x08};  // 2200 LE
    auto sr = dispatch_zcl_ep(c2, def, 0x0201, 1, "hvacThermostat",
                              attr_report(0x0012, 0x29, std::span<const std::uint8_t>(s.data(), s.size())));
    assert(sr.any_matched);
    assert(sr.merged.find("current_heating_setpoint") != nullptr);

    // SystemMode (0x001C, enum8) 4 = heat -> system_mode raw.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 1> m = {0x04};
    auto mr = dispatch_zcl_ep(c3, def, 0x0201, 1, "hvacThermostat",
                              attr_report(0x001C, 0x30, std::span<const std::uint8_t>(m.data(), m.size())));
    assert(mr.any_matched);
    assert(mr.merged.find("system_mode") != nullptr);

    // Phantom on/off must NOT decode a state key off this cluster.
    assert(tr.merged.find("state") == nullptr);
}

void test_mill_gen4() {
    using namespace zhc::devices::mill;
    check_thermostat_shape(kDef_Mill_gen_4);
    check_thermostat_decode(kDef_Mill_gen_4);
}

void test_mftwifi() {
    using namespace zhc::devices::mill;
    check_thermostat_shape(kDef_MFTWIFI);
    check_thermostat_decode(kDef_MFTWIFI);
}

}  // namespace

int main() {
    test_mill_gen4();
    test_mftwifi();
    return 0;
}
