// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Echostar "SAGE by Hughes" remotes.
// z2m-source: echostar.ts (fz.command_on/command_off, fz.SAGE206612_state).
//
// Bug fixed: both SAGE devices are battery-powered CONTROLLERS that EMIT
// genOnOff On/Off *commands* (device is the client) and surface them as an
// `action`; neither is controllable (z2m toZigbee is empty). The auto-port
// mis-modelled both as a settable on/off `state` (kFzOnOff + kTzOnOff) — a
// dead control — and:
//   * SAGE206611 lost its action stream (should be "on"/"off").
//   * SAGE206612 lost its action stream entirely (should be the doorbell's
//     "bell1"/"bell2", the device's only real signal).
//
// Both defs were graduated to Tier-2 parents:
//   * SAGE206611 wires the generic kFzCommandOn/Off (emit "on"/"off").
//   * SAGE206612 wires the vendor kFzSageBellAction (echostar/_shared.cpp,
//     the commandOn->"bell1" / commandOff->"bell2" map) + kFzBattery.
//
// These tests pin, on real genOnOff cluster-command wire shapes, that each
// command id produces the z2m-matching action, that the bogus on/off
// `state` is never emitted, and (206612) that battery still decodes.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::echostar {
extern const PreparedDefinition kDef_SAGE206611;  // single gang switch
extern const PreparedDefinition kDef_SAGE206612;  // doorbell sensor
}  // namespace zhc::devices::echostar

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

// 3-byte genOnOff cluster-specific command, client->server, no body:
//   fc(1)=0x01 (cluster-specific, C->S, not manu) | tsn(1) | cmd(1)
//   cmd 0x00 = Off, 0x01 = On.
struct CmdFrame { std::array<std::uint8_t, 3> bytes; };

CmdFrame on_off_cmd(std::uint8_t cmd, std::uint8_t tsn = 0x42) {
    CmdFrame f{};
    f.bytes = {0x01, tsn, cmd};
    return f;
}

DispatchResult run_cmd(const PreparedDefinition& def, std::uint8_t cmd) {
    const CmdFrame cf = on_off_cmd(cmd);
    auto raw = make_frame(0x0006 /* genOnOff */, 1,
                          std::span<const std::uint8_t>(cf.bytes.data(),
                                                        cf.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

constexpr std::uint8_t kCmdOff = 0x00;
constexpr std::uint8_t kCmdOn = 0x01;

// genPowerCfg batteryPercentageRemaining (attr 0x0021) report → battery%.
void assert_battery(const PreparedDefinition& def) {
    // fc=0x18 (profile-wide, S->C) | tsn | cmd=0x0A (report) |
    //   attr 0x0021 LE | type 0x20 (u8) | value 0xC8 (200 -> 100%).
    const std::array<std::uint8_t, 7> rep = {0x18, 0x01, 0x0A,
                                             0x21, 0x00, 0x20, 0xC8};
    auto raw = make_frame(0x0001 /* genPowerCfg */, 1,
                          std::span<const std::uint8_t>(rep.data(),
                                                        rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* b = r.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
}

}  // namespace

int main() {
    using namespace zhc::devices::echostar;

    // ── SAGE206611: single gang switch → action "on"/"off" ───────────
    {
        auto on = run_cmd(kDef_SAGE206611, kCmdOn);
        assert(on.any_matched);
        assert(action_is(on, "on"));
        // The bogus settable on/off state must never appear.
        assert(on.merged.find("state") == nullptr);

        auto off = run_cmd(kDef_SAGE206611, kCmdOff);
        assert(off.any_matched);
        assert(action_is(off, "off"));
        assert(off.merged.find("state") == nullptr);
    }

    // ── SAGE206612: doorbell → action "bell1"/"bell2" + battery ──────
    {
        auto bell1 = run_cmd(kDef_SAGE206612, kCmdOn);
        assert(bell1.any_matched);
        assert(action_is(bell1, "bell1"));
        // Must NOT collapse to the generic "on" literal, and no dead state.
        assert(!action_is(bell1, "on"));
        assert(bell1.merged.find("state") == nullptr);

        auto bell2 = run_cmd(kDef_SAGE206612, kCmdOff);
        assert(bell2.any_matched);
        assert(action_is(bell2, "bell2"));
        assert(!action_is(bell2, "off"));
        assert(bell2.merged.find("state") == nullptr);

        assert_battery(kDef_SAGE206612);
    }

    std::printf("echostar parity tests passed\n");
    return 0;
}
