// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Vimar (wired Italian) family.
// z2m-source: zigbee-herdsman-converters/src/devices/vimar.ts.
//
// Bugs fixed (vimar parity pass, 2026-06-10):
//
//   02973.B — z2m wires the full `fz.thermostat` and exposes a climate
//     with BOTH occupied_heating_setpoint and occupied_cooling_setpoint
//     plus system_mode off/heat/cool. The auto-port wired only the
//     generic kFzThermostat (decodes 0x0000/0x0012/0x001C), so the
//     occupied_cooling_setpoint (attr 0x0011) expose was dead, and the
//     expose declared system_mode as a phantom Binary. Now wires
//     kFzVimarThermostatExtras (0x0011 → occupied_cooling_setpoint)
//     alongside the generic decoder, exposes the cooling setpoint, and
//     corrects system_mode to an Enum.
//
//   RemoteControl_v1.0 — z2m role is `[m.commandsOnOff(),
//     m.commandsWindowCovering(), m.commandsLevelCtrl()]`: a battery
//     transmitter that EMITS actions. The auto-port mis-mapped it as a
//     controllable on/off switch (kFzOnOff + kTzOnOff + a `state`
//     expose) whose state never updates. Now wired to the generic
//     command converters (action emitter), with no phantom state and no
//     toZigbee.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::vimar {
extern const PreparedDefinition kDef_D02973_B;
extern const PreparedDefinition kDef_RemoteControl_v1_0;
}  // namespace zhc::devices::vimar

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
}

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

// ZCL attribute report (cluster-specific=false, Report=0x0A).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool int_is(const DispatchResult& r, const char* key, std::int64_t expect) {
    const Value* v = r.merged.find(key);
    if (!v) return false;
    if (v->type == ValueType::Int)  return v->i == expect;
    if (v->type == ValueType::Uint) return static_cast<std::int64_t>(v->u) == expect;
    return false;
}
bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}
bool def_exposes(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, name) == 0) return true;
    return false;
}
const Expose* find_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, name) == 0)
            return &def.exposes[i];
    return nullptr;
}

// ── 02973.B — thermostat with cooling-setpoint decode ────────────────
void test_thermostat() {
    const auto& def = devices::vimar::kDef_D02973_B;

    // Exposes: heating + cooling setpoints, local_temperature, system_mode
    // as an Enum (not the old phantom Binary).
    check(def_exposes(def, "local_temperature"),         "02973.B exposes local_temperature");
    check(def_exposes(def, "current_heating_setpoint"),  "02973.B exposes heating setpoint");
    check(def_exposes(def, "occupied_cooling_setpoint"), "02973.B exposes cooling setpoint");
    const Expose* sm = find_expose(def, "system_mode");
    check(sm != nullptr, "02973.B exposes system_mode");
    check(sm && sm->type == ExposeType::Enum, "02973.B system_mode is Enum (not phantom Binary)");

    // attr 0x0012 OccupiedHeatingSetpoint (int16, 2150 = 21.50 °C) → the
    // generic kFzThermostat still emits current_heating_setpoint.
    const std::array<std::uint8_t, 2> hsp{0x66, 0x08};  // 2150 LE
    auto h = attr_report(0x0012, 0x29, std::span<const std::uint8_t>(hsp.data(), hsp.size()));
    auto rh = dispatch_zcl(def, 0x0201, 1, std::span<const std::uint8_t>(h.data(), h.size()));
    check(int_is(rh, "current_heating_setpoint", 2150), "02973.B decodes heating setpoint (0x0012)");

    // attr 0x0011 OccupiedCoolingSetpoint (int16, 2400 = 24.00 °C) → the
    // previously-DEAD channel the vendor extras converter now wires.
    const std::array<std::uint8_t, 2> csp{0x60, 0x09};  // 2400 LE
    auto c = attr_report(0x0011, 0x29, std::span<const std::uint8_t>(csp.data(), csp.size()));
    auto rc = dispatch_zcl(def, 0x0201, 1, std::span<const std::uint8_t>(c.data(), c.size()));
    check(rc.any_matched, "02973.B cooling-setpoint report matches a converter");
    check(int_is(rc, "occupied_cooling_setpoint", 2400), "02973.B decodes cooling setpoint (0x0011)");

    // The thermostat is still settable (generic tz wired).
    check(def.to_zigbee_count >= 1, "02973.B retains thermostat setter");
}

// ── RemoteControl_v1.0 — action emitter (not an on/off switch) ───────
void test_remote_control() {
    const auto& def = devices::vimar::kDef_RemoteControl_v1_0;

    check(def_exposes(def, "action"),  "RemoteControl exposes action");
    check(!def_exposes(def, "state"),  "RemoteControl has no phantom state expose");
    check(def.to_zigbee_count == 0,    "RemoteControl is action-only (no tz)");
    check(def.from_zigbee_count >= 8,  "RemoteControl wires the command converters");

    // genOnOff `on` command (cluster-specific cmd 0x01) → action.
    const std::array<std::uint8_t, 3> onhdr{0x01, 0x42, 0x01};
    auto ron = dispatch_zcl(def, 0x0006, 1,
                            std::span<const std::uint8_t>(onhdr.data(), onhdr.size()));
    check(has_key(ron, "action"), "RemoteControl genOnOff on → action");

    // closuresWindowCovering `upOpen` command (cmd 0x00) → action.
    const std::array<std::uint8_t, 3> openhdr{0x01, 0x43, 0x00};
    auto rop = dispatch_zcl(def, 0x0102, 1,
                            std::span<const std::uint8_t>(openhdr.data(), openhdr.size()));
    check(has_key(rop, "action"), "RemoteControl cover open → action");

    // genLevelCtrl `step` command (cmd 0x02): step_mode=0 (up), size, tt LE.
    const std::array<std::uint8_t, 7> stephdr{0x01, 0x44, 0x02, 0x00, 0x32, 0x0A, 0x00};
    auto rst = dispatch_zcl(def, 0x0008, 1,
                            std::span<const std::uint8_t>(stephdr.data(), stephdr.size()));
    check(has_key(rst, "action"), "RemoteControl level step → action");
}

}  // namespace

int main() {
    std::printf("test_vimar_parity\n");
    test_thermostat();
    test_remote_control();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
