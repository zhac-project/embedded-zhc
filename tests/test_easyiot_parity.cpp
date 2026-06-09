// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Easyiot ZB-WB0x multi-button scene remotes.
// z2m-source: easyiot.ts — fzLocal.easyiot_action.
//
// Bug fixed: the four ZB-WB01/02/03/08 remotes lowered the generic
// kFzCommandOn/Off/Toggle converters, which emit the *standard* literals
// "on" / "off" / "toggle" with NO button prefix. z2m's `easyiot_action`
// instead (a) REMAPS the verbs — commandToggle->single, commandOn->double,
// commandOff->long — and (b) prefixes the originating button derived from
// `msg.endpoint.ID` ("1_single" .. "8_long"). So embedded-zhc emitted the
// wrong action verb AND lost the button identity (every button collapsed
// to one un-prefixed "action").
//
// All four defs were graduated to Tier-2 parents wiring the vendor
// kFzEasyiotAction converter (easyiot/_shared.cpp), which reproduces the
// z2m verb map + per-button prefix from src_endpoint.
//
// These tests pin, on real genOnOff cluster-command wire shapes, that
// each command id + endpoint produces the z2m-matching "<button>_<verb>"
// action, and that the bare "on"/"off"/"toggle" literals never appear.

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

namespace zhc::devices::easyiot {
extern const PreparedDefinition kDef_ZB_WB01;  // 1-button
extern const PreparedDefinition kDef_ZB_WB02;  // 2-button
extern const PreparedDefinition kDef_ZB_WB03;  // 3-button
extern const PreparedDefinition kDef_ZB_WB08;  // 8-button
}  // namespace zhc::devices::easyiot

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
//   cmd 0x00 = Off, 0x01 = On, 0x02 = Toggle.
struct CmdFrame { std::array<std::uint8_t, 3> bytes; };

CmdFrame on_off_cmd(std::uint8_t cmd, std::uint8_t tsn = 0x42) {
    CmdFrame f{};
    f.bytes = {0x01, tsn, cmd};
    return f;
}

DispatchResult run(const PreparedDefinition& def, std::uint8_t src_ep,
                   std::uint8_t cmd) {
    const CmdFrame cf = on_off_cmd(cmd);
    auto raw = make_frame(0x0006 /* genOnOff */, src_ep,
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

// genOnOff command ids.
constexpr std::uint8_t kCmdOff = 0x00;     // -> "long"
constexpr std::uint8_t kCmdOn = 0x01;      // -> "double"
constexpr std::uint8_t kCmdToggle = 0x02;  // -> "single"

void assert_button(const PreparedDefinition& def, std::uint8_t ep,
                   const char* btn) {
    char buf[16];

    // commandToggle -> "<btn>_single"
    auto tog = run(def, ep, kCmdToggle);
    assert(tog.any_matched);
    std::snprintf(buf, sizeof(buf), "%s_single", btn);
    assert(action_is(tog, buf));
    // The wrong (generic) verb must never appear.
    assert(!action_is(tog, "toggle"));

    // commandOn -> "<btn>_double"
    auto on = run(def, ep, kCmdOn);
    assert(on.any_matched);
    std::snprintf(buf, sizeof(buf), "%s_double", btn);
    assert(action_is(on, buf));
    assert(!action_is(on, "on"));

    // commandOff -> "<btn>_long"
    auto off = run(def, ep, kCmdOff);
    assert(off.any_matched);
    std::snprintf(buf, sizeof(buf), "%s_long", btn);
    assert(action_is(off, buf));
    assert(!action_is(off, "off"));
}

// genPowerCfg batteryPercentageRemaining (attr 0x0021) report → battery%.
// Confirms kFzBattery is still wired alongside the action converter.
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
    using namespace zhc::devices::easyiot;

    // WB01: single button on ep1.
    assert_button(kDef_ZB_WB01, 1, "1");
    assert_battery(kDef_ZB_WB01);

    // WB02: ep1 / ep2.
    assert_button(kDef_ZB_WB02, 1, "1");
    assert_button(kDef_ZB_WB02, 2, "2");
    assert_battery(kDef_ZB_WB02);

    // WB03: ep1..ep3.
    assert_button(kDef_ZB_WB03, 1, "1");
    assert_button(kDef_ZB_WB03, 3, "3");

    // WB08: full ep1..ep8 sweep — pins the per-button prefix table.
    for (std::uint8_t ep = 1; ep <= 8; ++ep) {
        char btn[2] = {static_cast<char>('0' + ep), '\0'};
        assert_button(kDef_ZB_WB08, ep, btn);
    }
    assert_battery(kDef_ZB_WB08);

    // Out-of-range endpoint (ep 9) must NOT produce an action (table is
    // bounded to 8 buttons; converter returns false → no match).
    {
        auto r = run(kDef_ZB_WB08, 9, kCmdOn);
        assert(r.merged.find("action") == nullptr);
    }

    std::printf("easyiot parity tests passed\n");
    return 0;
}
