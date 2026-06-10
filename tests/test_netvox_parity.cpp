// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Netvox family. The real z2m<->ez gap caught here:
//
//   * ZB02A "Wireless wall switch" is a remote/button: z2m wires
//     `fz.command_toggle` (genOnOff commandToggle) and exposes a single
//     `action: toggle`, with an empty toZigbee. The auto-generated port
//     mis-ported it as a CONTROLLABLE on/off switch — kFzOnOff (attribute
//     reporting) + kTzOnOff (a genOnOff write path) + a dead `state`
//     Binary StateSet expose — so every button press was dropped and the
//     UI offered a non-functional toggle target. Fixed by wiring the
//     generic kFzCommandToggle and publishing `action`, dropping the
//     write path and the phantom state.
//
//   * Z809A "Power socket with power consumption monitoring" is a
//     regression guard. z2m wires [fz.on_off, fz.electrical_measurement]
//     and exposes [switch, power, current, voltage] — the 0x0B04
//     (haElectricalMeasurement) half ONLY, no seMetering/energy. The ez
//     port already matches; this pins that the 0x0B04 decoder emits
//     power/voltage/current and that no phantom `energy` (0x0702) was
//     bolted on.
//
// z2m-source: zigbee-herdsman-converters/src/devices/netvox.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::netvox {
extern const PreparedDefinition kDef_ZB02A;  // wireless wall switch (remote)
extern const PreparedDefinition kDef_Z809A;  // metering power socket
}  // namespace zhc::devices::netvox

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
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01, tsn, cmd, payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> body) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : body) v.push_back(b);
    return v;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool action_is(const DispatchResult& r, const char* expect) {
    const Value* v = r.merged.find("action");
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
}

constexpr std::uint16_t GEN_ON_OFF  = 0x0006;
constexpr std::uint16_t HA_ELEC_MEAS = 0x0B04;
constexpr std::uint16_t SE_METERING  = 0x0702;

// ── ZB02A: command_toggle remote, not a controllable on/off ──────────
void test_zb02a_action() {
    const auto& def = devices::netvox::kDef_ZB02A;

    // Role is action: a single `action` enum expose, NO phantom state.
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    // A remote is a client — never a settable target.
    assert(def.to_zigbee == nullptr && def.to_zigbee_count == 0);
    // genOnOff is the action source cluster (bound for the toggle command).
    assert(def_binds(def, GEN_ON_OFF));

    // genOnOff Toggle (cmd 0x02) → action "toggle".
    auto rt = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x02, {}));
    assert(rt.any_matched);
    assert(action_is(rt, "toggle"));
    // Regression guard: an attribute report (the old kFzOnOff path) must
    // NOT produce a controllable `state` — only the command is decoded.
    const std::uint8_t kOnOffReport[] = {
        0x18, 0x43, 0x0A,   // fc, tsn, cmd=Report Attributes
        0x00, 0x00,         // attr 0x0000 (OnOff)
        0x10,               // type bool
        0x01,               // value 1
    };
    auto rr = dispatch_zcl(def, GEN_ON_OFF, 1, kOnOffReport);
    assert(rr.merged.find("state") == nullptr);
}

// ── Z809A: electrical-measurement (0x0B04) half, no energy/0x0702 ────
void test_z809a_electrical() {
    const auto& def = devices::netvox::kDef_Z809A;

    // z2m exposes [switch, power, current, voltage] — the 0x0B04 half.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    // No seMetering energy axis — z2m uses electrical_measurement only.
    assert(!def_exposes(def, "energy"));
    assert(def_binds(def, HA_ELEC_MEAS));
    assert(!def_binds(def, SE_METERING));

    // Report Attributes on 0x0B04: RMSVoltage 0x0505 (u16) = 230,
    // RMSCurrent 0x0508 (u16) = 5, ActivePower 0x050B (s16) = 1100.
    const std::uint8_t kElec[] = {
        0x18, 0x44, 0x0A,             // fc, tsn, cmd=Report Attributes
        0x05, 0x05, 0x21, 0xE6, 0x00, // 0x0505 u16 = 230
        0x08, 0x05, 0x21, 0x05, 0x00, // 0x0508 u16 = 5
        0x0B, 0x05, 0x29, 0x4C, 0x04, // 0x050B s16 = 1100
    };
    auto r = dispatch_zcl(def, HA_ELEC_MEAS, 1, kElec);
    assert(r.any_matched);
    const Value* volt = r.merged.find("voltage");
    const Value* curr = r.merged.find("current");
    const Value* powr = r.merged.find("power");
    assert(volt != nullptr);
    assert(curr != nullptr);
    assert(powr != nullptr);
    // Never decode an energy key off this cluster.
    assert(r.merged.find("energy") == nullptr);
}

}  // namespace

int main() {
    test_zb02a_action();
    test_z809a_electrical();
    return 0;
}
