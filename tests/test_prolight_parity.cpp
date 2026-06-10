// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Prolight family. Pins the two real gaps the
// auto-generated ports carried against the z2m wire shape:
//
//   * E27 (5412748727371) + GU10 (5412748727401) white-and-colour bulbs
//     are z2m m.light({colorTemp:{range:[153,555]}, color:true}) but the
//     auto-port dropped the 0x0300 lightingColorCtrl axis — color_temp +
//     color_xy exposes and the cluster bind were missing, so colour and
//     colour-temperature reports decoded to nothing. Fixed by wiring the
//     generic kFzColorTemperature/kFzColor (+ kTzColorTemp/kTzColor) and
//     restoring the color_temp/color_xy exposes and the {1,0x0300} bind.
//
//   * Remote (5412748727388) is an action-only ZLL transmitter
//     (toZigbee:[], fromZigbee command_on/off/move_to_level/move/stop/
//     move_to_color_temp/move_to_color/move_color_temperature + battery)
//     but was mis-ported as a controllable on/off switch (kFzOnOff +
//     kTzOnOff + a `state` expose). The `state` never updated (a remote
//     is the client of genOnOff) and the action stream was dead. Fixed by
//     wiring the generic Client->Server command converters + an `action`
//     enum, dropping the phantom state and the to_zigbee setter.
//
// z2m-source: zigbee-herdsman-converters/src/devices/prolight.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::prolight {
extern const PreparedDefinition kDef_D5412748727371;  // E27 white+colour
extern const PreparedDefinition kDef_D5412748727401;  // GU10 white+colour
extern const PreparedDefinition kDef_D5412748727388;  // remote control
}  // namespace zhc::devices::prolight

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

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}
bool action_is(const DispatchResult& r, const char* expect) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expect) == 0;
}
bool def_exposes(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, name) == 0) return true;
    return false;
}
bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// ── white-and-colour bulb: 0x0300 axis restored ─────────────────────
void check_colour_bulb(const PreparedDefinition& def, const char* tag) {
    check(def_exposes(def, "state"),      "bulb exposes state");
    check(def_exposes(def, "brightness"), "bulb exposes brightness");
    check(def_exposes(def, "color_temp"), "bulb exposes color_temp");
    check(def_exposes(def, "color_xy"),   "bulb exposes color_xy");
    check(def_binds(def, 0x0300),         "bulb binds lightingColorCtrl 0x0300");

    // lightingColorCtrl attr 0x0007 colorTemperature (u16 LE) -> color_temp.
    std::array<std::uint8_t, 2> ct{0x9A, 0x01};   // 410 mireds
    auto ctv = attr_report(0x0007, 0x21,
                           std::span<const std::uint8_t>(ct.data(), ct.size()));
    auto rct = dispatch_zcl(def, 0x0300, 1,
                            std::span<const std::uint8_t>(ctv.data(), ctv.size()));
    check(rct.any_matched,            "colorTemp report matched");
    check(has_key(rct, "color_temp"), "colorTemp report decodes color_temp");

    // lightingColorCtrl attr 0x0003 currentX (u16 LE) -> color_x.
    std::array<std::uint8_t, 2> cx{0x34, 0x12};
    auto cxv = attr_report(0x0003, 0x21,
                           std::span<const std::uint8_t>(cx.data(), cx.size()));
    auto rcx = dispatch_zcl(def, 0x0300, 1,
                            std::span<const std::uint8_t>(cxv.data(), cxv.size()));
    check(rcx.any_matched,         "currentX report matched");
    check(has_key(rcx, "color_x"), "currentX report decodes color_x");

    if (tag) { /* keep tag referenced for readability */ }
}

void test_e27_colour()  { check_colour_bulb(devices::prolight::kDef_D5412748727371, "E27"); }
void test_gu10_colour() { check_colour_bulb(devices::prolight::kDef_D5412748727401, "GU10"); }

// ── remote control: action stream live, no phantom on/off state ──────
void test_remote_action() {
    const auto& def = devices::prolight::kDef_D5412748727388;

    check(def_exposes(def, "action"),  "remote exposes action");
    check(def_exposes(def, "battery"), "remote exposes battery");
    check(!def_exposes(def, "state"),  "remote drops phantom on/off state");
    check(def.to_zigbee_count == 0,    "remote is action-only (no tz)");
    check(def.from_zigbee_count >= 8,  "remote wires the command converters");

    // genOnOff cluster-specific cmd 0x01 (on) -> action = "on".
    std::array<std::uint8_t, 3> onhdr{0x01, 0x42, 0x01};
    auto ron = dispatch_zcl(def, 0x0006, 1,
                            std::span<const std::uint8_t>(onhdr.data(), onhdr.size()));
    check(ron.any_matched,         "remote on command matched");
    check(action_is(ron, "on"),    "remote on command -> action=on");

    // genOnOff cluster-specific cmd 0x00 (off) -> action = "off".
    std::array<std::uint8_t, 3> offhdr{0x01, 0x43, 0x00};
    auto roff = dispatch_zcl(def, 0x0006, 1,
                             std::span<const std::uint8_t>(offhdr.data(), offhdr.size()));
    check(roff.any_matched,        "remote off command matched");
    check(action_is(roff, "off"),  "remote off command -> action=off");

    // genLevelCtrl cluster-specific cmd 0x01 (move) payload [mode,rate]
    //   mode 0 = up -> action = "brightness_move_up".
    std::array<std::uint8_t, 5> movehdr{0x01, 0x44, 0x01, 0x00, 0x32};
    auto rmove = dispatch_zcl(def, 0x0008, 1,
                              std::span<const std::uint8_t>(movehdr.data(), movehdr.size()));
    check(rmove.any_matched,                          "remote move command matched");
    check(action_is(rmove, "brightness_move_up"),     "remote move(up) -> action=brightness_move_up");

    // Regression guard: the old port wired kFzOnOff (an attribute decoder
    // on genOnOff onOff=0x0000) + a controllable `state`. A genOnOff
    // *attribute report* of onOff must NOT now resurrect a state key.
    std::array<std::uint8_t, 1> onoff{0x01};
    auto rep = attr_report(0x0000, 0x10,
                           std::span<const std::uint8_t>(onoff.data(), onoff.size()));
    auto rstate = dispatch_zcl(def, 0x0006, 1,
                               std::span<const std::uint8_t>(rep.data(), rep.size()));
    check(!has_key(rstate, "state"), "remote never emits a controllable state");
}

}  // namespace

int main() {
    test_e27_colour();
    test_gu10_colour();
    test_remote_action();
    if (g_failures) {
        std::printf("prolight parity: %d FAILED\n", g_failures);
        return 1;
    }
    std::printf("prolight parity: all checks passed\n");
    return 0;
}
