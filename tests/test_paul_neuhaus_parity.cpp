// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for Paul Neuhaus (Q-Home line).
// z2m-source: zigbee-herdsman-converters/src/devices/paul_neuhaus.ts.
//
// The lighting SKUs (NLG-*, Neuhaus *, JZ*-light, 100.*) are plain
// m.light()/m.onOff() ports and need no parity coverage. The two REMOTES
// are the suspects, and both carried stale "converter absent" comments
// from an era before the generic command bundle gained colour-wheel
// decoders:
//
//   1. 100.462.31 (Q-REMOTE) — z2m wires command_color_loop_set and
//      command_enhanced_move_to_hue_and_saturation, but the port skipped
//      them claiming kFzCommandColorLoopSet /
//      kFzCommandEnhancedMoveToHueAndSat were missing. They exist now
//      (lightingColorCtrl cmd 0x44 / 0x43); wiring them makes the
//      `color_loop_set` and `enhanced_move_to_hue_and_saturation` enum
//      actions surface. (`scene_*` stays an INFRA defer — the genBasic
//      Write-Attributes parser does not populate msg.payload.)
//
//   2. E0040006 (JZ-RC-J4R, Q RGBW remote) — the port SUBSTITUTED
//      kFzCommandMoveToHueAndSaturation (emits "move_to_hue_and_saturation")
//      for z2m's command_enhanced_move_to_hue_and_saturation, and dropped
//      command_color_loop_set. Fixed by wiring the correct decoders; the
//      wrong MoveToHueAndSaturation (cmd 0x06) must no longer be claimed.

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

namespace zhc::devices::paul_neuhaus {
extern const PreparedDefinition kDef_D100_462_31;  // Q-REMOTE
extern const PreparedDefinition kDef_E0040006;     // JZ-RC-J4R Q RGBW remote
}  // namespace zhc::devices::paul_neuhaus

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

DispatchResult dispatch_zcl(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t cluster_id, std::uint8_t src_ep,
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd,
// then the payload.
std::array<std::uint8_t, 16> g_cmd_buf;
std::span<const std::uint8_t> cmd_frame(std::uint8_t cmd,
                                        std::span<const std::uint8_t> payload) {
    std::size_t i = 0;
    g_cmd_buf[i++] = 0x01;
    g_cmd_buf[i++] = 0x42;
    g_cmd_buf[i++] = cmd;
    for (auto b : payload) g_cmd_buf[i++] = b;
    return std::span<const std::uint8_t>(g_cmd_buf.data(), i);
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── lightingColorCtrl ColorLoopSet (cmd 0x44): updateflags u8, action u8,
//    direction u8, time u16le, startHue u16le. action=1 → LUT entry 1. ──────
DispatchResult color_loop_set(RuntimeContext& ctx, const PreparedDefinition& def) {
    const std::array<std::uint8_t, 7> p = {0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    return dispatch_zcl(ctx, def, 0x0300, 1, "lightingColorCtrl",
                        cmd_frame(0x44, std::span<const std::uint8_t>(p.data(), p.size())));
}

// ── lightingColorCtrl EnhancedMoveToHueAndSaturation (cmd 0x43):
//    enhancehue u16le, saturation u8, transtime u16le. ──────────────────────
DispatchResult enhanced_move_to_hue_sat(RuntimeContext& ctx,
                                        const PreparedDefinition& def) {
    const std::array<std::uint8_t, 5> p = {0x00, 0x80, 0xFE, 0x0A, 0x00};
    return dispatch_zcl(ctx, def, 0x0300, 1, "lightingColorCtrl",
                        cmd_frame(0x43, std::span<const std::uint8_t>(p.data(), p.size())));
}

// ─────────────────────────────────────────────────────────────────────────
// 100.462.31 (Q-REMOTE): color_loop_set + enhanced_move_to_hue_and_saturation
// now decode; the baseline on/off action still works.
// ─────────────────────────────────────────────────────────────────────────
void check_q_remote() {
    const auto& def = zhc::devices::paul_neuhaus::kDef_D100_462_31;
    RuntimeContext ctx{};

    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "action_group"));
    assert(def.to_zigbee_count == 0);  // receive-only remote

    // genOnOff On (cmd 0x01) → action="on" (regression guard on the base set).
    auto on = dispatch_zcl(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x01, {}));
    assert(on.any_matched);
    assert(action_is(on, "on"));

    // NEW: color_loop_set (action byte 1 → "activate_from_color_loop_start_enhanced_hue").
    auto cl = color_loop_set(ctx, def);
    assert(cl.any_matched);
    assert(action_is(cl, "activate_from_color_loop_start_enhanced_hue"));

    // NEW: enhanced_move_to_hue_and_saturation.
    auto en = enhanced_move_to_hue_sat(ctx, def);
    assert(en.any_matched);
    assert(action_is(en, "enhanced_move_to_hue_and_saturation"));
}

// ─────────────────────────────────────────────────────────────────────────
// E0040006 (JZ-RC-J4R): the correct enhanced decoder replaces the wrong
// MoveToHueAndSaturation, and color_loop_set is now wired.
// ─────────────────────────────────────────────────────────────────────────
void check_q_rgbw_remote() {
    const auto& def = zhc::devices::paul_neuhaus::kDef_E0040006;
    RuntimeContext ctx{};

    assert(def_exposes(def, "action"));
    assert(def.to_zigbee_count == 0);

    // NEW: the correct z2m action label is emitted (was "move_to_hue_and_saturation").
    auto en = enhanced_move_to_hue_sat(ctx, def);
    assert(en.any_matched);
    assert(action_is(en, "enhanced_move_to_hue_and_saturation"));

    // NEW: color_loop_set decodes.
    auto cl = color_loop_set(ctx, def);
    assert(cl.any_matched);
    assert(action_is(cl, "activate_from_color_loop_start_enhanced_hue"));

    // Regression: the wrong decoder is gone. A plain MoveToHueAndSaturation
    // (cmd 0x06) must NOT be claimed now that the def no longer wires it.
    const std::array<std::uint8_t, 4> hs = {0x80, 0xFE, 0x0A, 0x00};
    auto wrong = dispatch_zcl(ctx, def, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x06, std::span<const std::uint8_t>(hs.data(), hs.size())));
    assert(wrong.merged.find("action") == nullptr);

    // Baseline command_step still works (brightness_step_up, mode 0).
    const std::array<std::uint8_t, 4> step = {0x00, 0x20, 0x0A, 0x00};
    auto st = dispatch_zcl(ctx, def, 0x0008, 1, "genLevelCtrl",
                           cmd_frame(0x02, std::span<const std::uint8_t>(step.data(), step.size())));
    assert(st.any_matched);
    assert(action_is(st, "brightness_step_up"));
}

}  // namespace

int main() {
    check_q_remote();
    check_q_rgbw_remote();
    std::printf("paul_neuhaus parity tests passed\n");
    return 0;
}
