// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Lytko L101Ze thermostat family.
//
// Bug fixed (wrong device class): z2m defines every L101Ze-* model via
// `modernExtend`'s `thermostat({...})` builder (a floor/room heating
// thermostat — local_temperature / occupied_heating_setpoint /
// system_mode / running_mode on hvacThermostat, plus a few Lytko
// manuSpec config attrs). The legacy generator cannot read those
// dynamic exposes, so SIX of the seven L101Ze ports were lowered to an
// incorrect battery + onOff stub (exposes state/battery/voltage, wired
// to kFzBattery + kFzOnOff, bound to genOnOff 0x0006). On real hardware
// those defs would never surface a temperature, setpoint or mode — the
// device's entire purpose. (The seventh, L101Ze-SBN, had already been
// hand-corrected on 2026-04-28; this sweep brings the other six —
// SLN, SLM, SMN, DLN, DLM, DBN — to the same canonical shape.)
//
// Each was graduated to a Tier-2 parent override that maps the core
// control surface onto the generic kFzThermostat decoder (0x0000 local
// temp / 0x0012 setpoint / 0x001C system mode) + generic kTzThermostat
// writes + the shared Lytko manuSpec TZ encoders. Dual-channel models
// (D*) declare a two-entry endpoint_map {3,4} so the runtime suffixes
// per-channel keys to `<key>_3` / `<key>_4`.
//
// This test pins, on real hvacThermostat attribute-report wire shapes:
//   * single-channel L101Ze-SLN decodes local_temperature /
//     current_heating_setpoint / system_mode and is NOT a switch
//     (no "state" / "battery" expose);
//   * dual-channel L101Ze-DBN decodes the SAME surface suffixed per
//     endpoint (`local_temperature_3` on ep3, `..._4` on ep4) — the
//     two channels do not collide.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lytko.ts
// #L101Ze-SLN / #L101Ze-DBN (and the SLM/SMN/DLN/DLM siblings).

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

namespace zhc::devices::lytko {
extern const PreparedDefinition kDef_L101Ze_SLN;   // single channel, no display
extern const PreparedDefinition kDef_L101Ze_DBN;   // dual channel, big display
}  // namespace zhc::devices::lytko

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

// Plain ZCL attribute report: fc=0x18, tsn, cmd=0x0A, then records.
// 0x0000 / 0x0012 / 0x001C are standard hvacThermostat attrs reported
// without a manufacturer header (matching z2m fz.thermostat).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── L101Ze-SLN: single channel thermostat (was a battery/onOff stub) ──
void check_sln() {
    const auto& def = devices::lytko::kDef_L101Ze_SLN;

    // Core thermostat surface is declared …
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    // … and the wrong-class stub keys are gone.
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));

    // attr 0x0000 LocalTemperature, s16 (type 0x29) = 0x0834 = 2100 → 21.00 °C raw.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", 3,
                           attr_report(0x0000, 0x29, lt));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // attr 0x0012 OccupiedHeatingSetpoint, s16 = 0x07D0 = 2000 → current_heating_setpoint.
    const std::uint8_t sp[] = {0xD0, 0x07};
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat", 3,
                           attr_report(0x0012, 0x29, sp));
    const Value* sv = rs.merged.find("current_heating_setpoint");
    assert(sv && sv->type == ValueType::Int && sv->i == 2000);

    // attr 0x001C SystemMode, u8 enum = 4 (heat).
    const std::uint8_t sm[] = {0x04};
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat", 3,
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 4);
}

// ── L101Ze-DBN: dual channel — per-endpoint key suffixing (_3 / _4) ──
void check_dbn() {
    const auto& def = devices::lytko::kDef_L101Ze_DBN;

    // Per-channel exposes declared with the suffixes the runtime emits.
    assert(def_exposes(def, "local_temperature_3"));
    assert(def_exposes(def, "local_temperature_4"));
    assert(def_exposes(def, "current_heating_setpoint_3"));
    assert(def_exposes(def, "system_mode_4"));
    assert(!def_exposes(def, "state"));

    // Channel 3: localTemp = 0x0834 = 2100 must land on local_temperature_3.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto r3 = dispatch_zcl(def, 0x0201, "hvacThermostat", 3,
                           attr_report(0x0000, 0x29, lt));
    assert(r3.any_matched);
    assert(r3.merged.find("local_temperature_3"));
    assert(!r3.merged.find("local_temperature"));   // un-suffixed key must NOT exist
    assert(!r3.merged.find("local_temperature_4"));  // wrong channel must NOT exist
    const Value* t3 = r3.merged.find("local_temperature_3");
    assert(t3 && t3->type == ValueType::Int && t3->i == 2100);

    // Channel 4: setpoint = 0x08FC = 2300 must land on current_heating_setpoint_4.
    const std::uint8_t sp[] = {0xFC, 0x08};
    auto r4 = dispatch_zcl(def, 0x0201, "hvacThermostat", 4,
                           attr_report(0x0012, 0x29, sp));
    assert(r4.any_matched);
    const Value* s4 = r4.merged.find("current_heating_setpoint_4");
    assert(s4 && s4->type == ValueType::Int && s4->i == 2300);
    assert(!r4.merged.find("current_heating_setpoint_3"));
}

}  // namespace

int main() {
    check_sln();
    check_dbn();
    return 0;
}
