// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Stelpro (Quebec) line-voltage heating-thermostat
// family. One real gap in the auto-generated ports is pinned here:
//
//   * keypad_lockout dead attribute. All six defs (HT402, SMT402,
//     SMT402AD, SORB, ST218, STZB402) declared the `keypad_lockout`
//     expose as StateSet (read + write) but wired neither a decoder nor
//     an encoder for it. z2m wires `fz.hvac_user_interface`
//     (hvacUserInterfaceCfg 0x0204 attr 0x0001 keypadLockout, ENUM8 →
//     keypad_lockout) and `tz.thermostat_keypad_lockout` (write that
//     attr) on every Stelpro device, so the value never decoded and
//     could not be set. Fixed by the new kFzStelproKeypadLockout +
//     kTzStelproKeypadLockout, wired into all six (graduated to parent).
//
// The pre-existing Stelpro converters are guarded too: the
// hvacThermostat decoder (local_temperature, current_heating_setpoint,
// system_mode + Eco(5)→"auto" override, running_state derived from
// pIHeatingDemand 0x0008), and the outdoor-temperature manuSpec write
// (×100 scaling, mfgcode 0x1185).
//
// z2m-source: zigbee-herdsman-converters/src/devices/stelpro.ts.

#include <array>
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

namespace zhc::devices::stelpro {
extern const PreparedDefinition kDef_HT402;
extern const PreparedDefinition kDef_SMT402;
extern const PreparedDefinition kDef_SMT402AD;
extern const PreparedDefinition kDef_SORB;
extern const PreparedDefinition kDef_ST218;
extern const PreparedDefinition kDef_STZB402;
}  // namespace zhc::devices::stelpro

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
    auto raw = make_frame(cluster_id, 25, bytes);   // Stelpro reports on ep25
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

// ── keypad_lockout: read (0x0204/0x0001 ENUM8) + write ────────────────
void check_keypad_lockout(const PreparedDefinition& def) {
    assert(def_exposes(def, "keypad_lockout"));

    // Decode: hvacUserInterfaceCfg attr 0x0001 (ENUM8 0x30) == 2 (lock2).
    const std::uint8_t two[] = {0x02};
    auto r = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg",
                          attr_report(0x0001, 0x30, two));
    assert(r.any_matched);
    const Value* k = r.merged.find("keypad_lockout");
    assert(k && k->type == ValueType::Uint && k->u == 2);

    // unlock (0) decodes too.
    const std::uint8_t zero[] = {0x00};
    auto r0 = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg",
                           attr_report(0x0001, 0x30, zero));
    assert(r0.any_matched);
    const Value* k0 = r0.merged.find("keypad_lockout");
    assert(k0 && k0->type == ValueType::Uint && k0->u == 0);

    // Write path: keypad_lockout = 1 → writeAttributes on 0x0204, attr
    // 0x0001, type ENUM8 (0x30), value 1.
    RuntimeContext ctx{};
    std::array<std::uint8_t, 16> frame{};
    Value v{}; v.type = ValueType::Uint; v.u = 1;
    auto w = dispatch_to_zigbee(def, "keypad_lockout", v, ctx, frame);
    assert(w.ok);
    assert(w.cluster_id == 0x0204);
    assert(w.command_id == 0x02);             // writeAttributes
    // frame = fc, tsn, cmd, attr_lo, attr_hi, type, value...
    assert(frame[2] == 0x02);                 // writeAttributes command
    assert(frame[3] == 0x01 && frame[4] == 0x00);  // attr 0x0001
    assert(frame[5] == 0x30);                 // ENUM8
    assert(frame[6] == 0x01);                 // value 1
}

// ── hvacThermostat: standard surface + Stelpro tweaks ─────────────────
void check_thermostat(const PreparedDefinition& def) {
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "running_state"));

    // LocalTemperature (0x0000 s16) = 0x0834 = 2100 (21.00 °C raw int).
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0000, 0x29, lt));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // SystemMode (0x001C u8) raw enum passes through.
    const std::uint8_t sm[] = {0x04};
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001C, 0x30, sm));
    assert(rs.any_matched);
    assert(rs.merged.find("system_mode") != nullptr);

    // stelproSystemMode (0x401C) == 5 → system_mode "auto" (Eco).
    const std::uint8_t eco[] = {0x05};
    auto re = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x401C, 0x30, eco));
    assert(re.any_matched);
    assert(str_is(re.merged.find("system_mode"), "auto"));

    // pIHeatingDemand (0x0008 u8): 25 → running_state "heat"; 0 → "idle".
    const std::uint8_t d_hi[] = {25};
    auto rh = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, d_hi));
    assert(rh.any_matched);
    assert(str_is(rh.merged.find("running_state"), "heat"));

    const std::uint8_t d_lo[] = {5};
    auto rl = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, d_lo));
    assert(str_is(rl.merged.find("running_state"), "idle"));
}

// ── outdoor_temperature_display: write ×100 (mfg 0x1185) ──────────────
void check_outdoor_temp(const PreparedDefinition& def) {
    assert(def_exposes(def, "outdoor_temperature_display"));
    RuntimeContext ctx{};
    std::array<std::uint8_t, 24> frame{};
    Value v{}; v.type = ValueType::Int; v.i = 7;       // 7 °C → raw 700
    auto w = dispatch_to_zigbee(def, "outdoor_temperature_display", v, ctx, frame);
    assert(w.ok);
    assert(w.cluster_id == 0x0201);
    assert(w.command_id == 0x02);                      // writeAttributes
}

}  // namespace

int main() {
    const PreparedDefinition* all[] = {
        &devices::stelpro::kDef_HT402,
        &devices::stelpro::kDef_SMT402,
        &devices::stelpro::kDef_SMT402AD,
        &devices::stelpro::kDef_SORB,
        &devices::stelpro::kDef_ST218,
        &devices::stelpro::kDef_STZB402,
    };

    // keypad_lockout decode + write on every def (the fixed gap).
    for (const auto* d : all) check_keypad_lockout(*d);

    // Standard thermostat surface + Stelpro tweaks on every def.
    for (const auto* d : all) check_thermostat(*d);

    // outdoor_temperature_display write on the five defs that wire it
    // (every def except SORB, which z2m does not give the outdoor tz).
    check_outdoor_temp(devices::stelpro::kDef_HT402);
    check_outdoor_temp(devices::stelpro::kDef_SMT402);
    check_outdoor_temp(devices::stelpro::kDef_SMT402AD);
    check_outdoor_temp(devices::stelpro::kDef_ST218);
    check_outdoor_temp(devices::stelpro::kDef_STZB402);
    assert(!def_exposes(devices::stelpro::kDef_SORB, "outdoor_temperature_display"));

    return 0;
}
