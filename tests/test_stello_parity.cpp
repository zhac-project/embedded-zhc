// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Stello (Hilo) family. Two devices:
//
//   * STLO-34 — Hilo thermostat. One real gap pinned here: the
//     keypad_lockout dead attribute. The auto-generated port declared
//     the `keypad_lockout` expose as StateSet (read + write) but wired
//     neither a decoder nor an encoder. z2m wires fz.hvac_user_interface
//     (hvacUserInterfaceCfg 0x0204 attr 0x0001 keypadLockout ENUM8 →
//     keypad_lockout) and tz.thermostat_keypad_lockout (write that attr),
//     so the value never decoded and could not be set. Fixed by wiring
//     the shared kFzStelproKeypadLockout + kTzStelproKeypadLockout
//     (graduated to parent). The Hilo power/energy decoders on
//     hvacThermostat 0x4008/0x4009 and the Stelpro thermostat surface
//     (local_temperature, system_mode, running_state from pIHeatingDemand)
//     are guarded too.
//
//   * STLO-23 — Hilo water heater controller. onOff + seMetering
//     ("metering" cluster = 0x0702 only, no 0x0B04) + peak_demand_icon
//     write. No gap; the surface is pinned for regression.
//
// z2m-source: zigbee-herdsman-converters/src/devices/stello.ts.

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

namespace zhc::devices::stello {
extern const PreparedDefinition kDef_STLO_23;
extern const PreparedDefinition kDef_STLO_34;
}  // namespace zhc::devices::stello

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

// ── STLO-34: keypad_lockout dead-attribute fix (decode + write) ───────
void check_stlo34_keypad_lockout() {
    const PreparedDefinition& def = devices::stello::kDef_STLO_34;
    assert(def_exposes(def, "keypad_lockout"));

    // Decode: hvacUserInterfaceCfg attr 0x0001 (ENUM8 0x30) == 2.
    const std::uint8_t two[] = {0x02};
    auto r = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg", 25,
                          attr_report(0x0001, 0x30, two));
    assert(r.any_matched);
    const Value* k = r.merged.find("keypad_lockout");
    assert(k && k->type == ValueType::Uint && k->u == 2);

    // unlock (0) decodes too.
    const std::uint8_t zero[] = {0x00};
    auto r0 = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg", 25,
                           attr_report(0x0001, 0x30, zero));
    assert(r0.any_matched);
    const Value* k0 = r0.merged.find("keypad_lockout");
    assert(k0 && k0->type == ValueType::Uint && k0->u == 0);

    // Write path: keypad_lockout = 1 → writeAttributes on 0x0204.
    RuntimeContext ctx{};
    std::array<std::uint8_t, 16> frame{};
    Value v{}; v.type = ValueType::Uint; v.u = 1;
    auto w = dispatch_to_zigbee(def, "keypad_lockout", v, ctx, frame);
    assert(w.ok);
    assert(w.cluster_id == 0x0204);
    assert(w.command_id == 0x02);             // writeAttributes
    assert(frame[2] == 0x02);                 // writeAttributes command
    assert(frame[3] == 0x01 && frame[4] == 0x00);  // attr 0x0001
    assert(frame[5] == 0x30);                 // ENUM8
    assert(frame[6] == 0x01);                 // value 1
}

// ── STLO-34: Stelpro thermostat surface + Hilo power/energy ───────────
void check_stlo34_thermostat() {
    const PreparedDefinition& def = devices::stello::kDef_STLO_34;
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "running_state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));

    // LocalTemperature (0x0000 s16) = 0x0834 = 2100 (21.00 °C raw int).
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat", 25,
                           attr_report(0x0000, 0x29, lt));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // pIHeatingDemand (0x0008 u8): 25 → running_state "heat"; 0 → "idle".
    const std::uint8_t d_hi[] = {25};
    auto rh = dispatch_zcl(def, 0x0201, "hvacThermostat", 25,
                           attr_report(0x0008, 0x20, d_hi));
    assert(rh.any_matched);
    assert(str_is(rh.merged.find("running_state"), "heat"));

    const std::uint8_t d_lo[] = {0};
    auto rl = dispatch_zcl(def, 0x0201, "hvacThermostat", 25,
                           attr_report(0x0008, 0x20, d_lo));
    assert(str_is(rl.merged.find("running_state"), "idle"));

    // power: hvacThermostat 0x4008 (u16 W) == 0x0064 = 100.
    const std::uint8_t pw[] = {0x64, 0x00};
    auto rp = dispatch_zcl(def, 0x0201, "hvacThermostat", 25,
                           attr_report(0x4008, 0x21, pw));
    assert(rp.any_matched);
    const Value* pv = rp.merged.find("power");
    assert(pv && pv->type == ValueType::Uint && pv->u == 100);

    // energy: hvacThermostat 0x4009 (u32 Wh) == 5000 → 5.0 kWh.
    const std::uint8_t en[] = {0x88, 0x13, 0x00, 0x00};  // 5000
    auto reu = dispatch_zcl(def, 0x0201, "hvacThermostat", 25,
                            attr_report(0x4009, 0x23, en));
    assert(reu.any_matched);
    const Value* ev = reu.merged.find("energy");
    assert(ev && ev->type == ValueType::Float && ev->f > 4.99f && ev->f < 5.01f);
}

// ── STLO-34: outdoor_temperature_display write (mfg 0x1185) ───────────
void check_stlo34_outdoor_temp() {
    const PreparedDefinition& def = devices::stello::kDef_STLO_34;
    assert(def_exposes(def, "outdoor_temperature_display"));
    RuntimeContext ctx{};
    std::array<std::uint8_t, 24> frame{};
    Value v{}; v.type = ValueType::Int; v.i = 7;       // 7 °C → raw 700
    auto w = dispatch_to_zigbee(def, "outdoor_temperature_display", v, ctx, frame);
    assert(w.ok);
    assert(w.cluster_id == 0x0201);
    assert(w.command_id == 0x02);                      // writeAttributes
}

// ── STLO-23: onOff + metering (0x0702) + peak_demand_icon write ───────
void check_stlo23() {
    const PreparedDefinition& def = devices::stello::kDef_STLO_23;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "peak_demand_icon"));

    // onOff (genOnOff 0x0006 attr 0x0000 bool) == 1 → state true.
    const std::uint8_t on[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                           attr_report(0x0000, 0x10, on));
    assert(ro.any_matched);
    const Value* sv = ro.merged.find("state");
    assert(sv && sv->type == ValueType::Bool && sv->b == true);

    // metering: seMetering 0x0702 currentSummDelivered (0x0000 u48).
    const std::uint8_t es[] = {0x10, 0x27, 0x00, 0x00, 0x00, 0x00};  // 10000
    auto rm = dispatch_zcl(def, 0x0702, "seMetering", 1,
                           attr_report(0x0000, 0x25, es));
    assert(rm.any_matched);

    // peak_demand_icon write → manu writeAttributes on 0x0201 (mfg 0x1185).
    RuntimeContext ctx{};
    std::array<std::uint8_t, 24> frame{};
    Value v{}; v.type = ValueType::Uint; v.u = 6;
    auto w = dispatch_to_zigbee(def, "peak_demand_icon", v, ctx, frame);
    assert(w.ok);
    assert(w.cluster_id == 0x0201);
    assert(w.command_id == 0x02);                      // writeAttributes
}

}  // namespace

int main() {
    check_stlo34_keypad_lockout();
    check_stlo34_thermostat();
    check_stlo34_outdoor_temp();
    check_stlo23();
    return 0;
}
