// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Müller-Licht (tint) battery remotes. The three
// remotes shipped action enums that advertised colour-wheel + scene +
// store events, but the auto-generated ports only wired the
// genOnOff/genLevelCtrl command decoders — so several advertised action
// strings were dead enum entries no inbound frame could ever produce.
//
// z2m-source: zigbee-herdsman-converters/src/devices/muller_licht.ts.
//
// Fixed (decoder wired, value now surfaces at the dispatch boundary):
//   * MLI-404011/MLI-404049 — color_temperature_move (lightingColorCtrl
//     MoveToColorTemp 0x0A) + color_move (MoveToColor 0x07). z2m uses
//     fz.tint404011_move_to_color_temp / fz.command_move_to_color; the
//     bare action strings match (the z2m _up/_down suffix is internal
//     state-tracking, not on the wire).
//   * 404022/404049C — color_move (MoveToColor 0x07). color_temperature_move
//     was already wired.
//   * 404002 — store_1 (genScenes commandStore 0x04). recall_1 already
//     worked via kFzCommandRecall.
//
// Deliberately NOT fixed (pinned as a known gap): scene_<n> for both
// colour remotes. z2m's fz.tint_scene decodes a genBasic *Write
// Attributes* of attr 0x4005, but the foundation Write-attribute parser
// does not populate DecodedMessage::payload (the body stays in raw_data),
// so the generic kFzTintScene converter can never fire. This is an INFRA
// gap (shared Write-attr parsing), not a per-device port; the assertion
// below pins the current behaviour so a future parser fix flips it
// intentionally rather than silently.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::muller_licht {
extern const PreparedDefinition kDef_MLI_404011_MLI_404049;  // 4-button RGB+CCT remote
extern const PreparedDefinition kDef_D404022_404049C;        // 10-scene RGB+CCT remote
extern const PreparedDefinition kDef_D404002;                // dim remote (recall/store)
}  // namespace zhc::devices::muller_licht

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

const char* action_of(const DispatchResult& r) {
    const Value* a = r.merged.find("action");
    if (!a || a->type != ValueType::StringRef) return nullptr;
    return a->str;
}

// Dispatch a cluster-specific command frame (client->server, fc=0x01).
//   bytes = full ZCL payload: fc(1)=0x01 | tsn(1) | cmd(1) | body...
DispatchResult dispatch_cmd(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── MLI-404011/MLI-404049 — 4-button RGB+CCT remote ──────────────────
void test_404011_colour_wheel() {
    const auto& def = devices::muller_licht::kDef_MLI_404011_MLI_404049;
    assert(def_exposes(def, "action") && !def_exposes(def, "state"));
    assert(def_binds(def, 0x0300));   // lightingColorCtrl bound

    // genOnOff On (0x01) still works.
    const std::array<std::uint8_t, 3> on = {0x01, 0x42, 0x01};
    auto ron = dispatch_cmd(def, 0x0006, 1, {on.data(), on.size()});
    assert(ron.any_matched);
    const char* aon = action_of(ron);
    assert(aon && std::strcmp(aon, "on") == 0);

    // lightingColorCtrl MoveToColorTemp (0x0A): ct=370 LE, transition=5
    // -> bare "color_temperature_move".
    const std::array<std::uint8_t, 7> ct = {0x01, 0x42, 0x0A, 0x72, 0x01, 0x05, 0x00};
    auto rct = dispatch_cmd(def, 0x0300, 1, {ct.data(), ct.size()});
    assert(rct.any_matched);
    const char* act_ct = action_of(rct);
    assert(act_ct && std::strcmp(act_ct, "color_temperature_move") == 0);

    // lightingColorCtrl MoveToColor (0x07): cx=0x1234, cy=0x5678, tt=10
    // -> "color_move" + action_color_x/y.
    const std::array<std::uint8_t, 9> cm =
        {0x01, 0x42, 0x07, 0x34, 0x12, 0x78, 0x56, 0x0A, 0x00};
    auto rcm = dispatch_cmd(def, 0x0300, 1, {cm.data(), cm.size()});
    assert(rcm.any_matched);
    const char* act_cm = action_of(rcm);
    assert(act_cm && std::strcmp(act_cm, "color_move") == 0);
    const Value* cx = rcm.merged.find("action_color_x");
    assert(cx && cx->type == ValueType::Uint && cx->u == 0x1234);
}

// ── 404022/404049C — 10-scene RGB+CCT remote ─────────────────────────
void test_404022_colour_wheel() {
    const auto& def = devices::muller_licht::kDef_D404022_404049C;
    assert(def_exposes(def, "action") && !def_exposes(def, "state"));
    assert(def_binds(def, 0x0300));

    // color_temperature_move (already wired before this pass) — sanity.
    const std::array<std::uint8_t, 7> ct = {0x01, 0x42, 0x0A, 0x72, 0x01, 0x05, 0x00};
    auto rct = dispatch_cmd(def, 0x0300, 1, {ct.data(), ct.size()});
    assert(rct.any_matched);
    const char* act_ct = action_of(rct);
    assert(act_ct && std::strcmp(act_ct, "color_temperature_move") == 0);

    // color_move — the gap fixed this pass.
    const std::array<std::uint8_t, 9> cm =
        {0x01, 0x42, 0x07, 0x34, 0x12, 0x78, 0x56, 0x0A, 0x00};
    auto rcm = dispatch_cmd(def, 0x0300, 1, {cm.data(), cm.size()});
    assert(rcm.any_matched);
    const char* act_cm = action_of(rcm);
    assert(act_cm && std::strcmp(act_cm, "color_move") == 0);
}

// ── 404002 — dim remote: recall + store ──────────────────────────────
void test_404002_recall_store() {
    const auto& def = devices::muller_licht::kDef_D404002;
    assert(def_exposes(def, "action") && !def_exposes(def, "state"));
    assert(def_binds(def, 0x0005));   // genScenes bound

    // genScenes Recall (0x05): group=0x0001 LE, scene=1 -> "recall_1"
    // (already worked before this pass — sanity).
    const std::array<std::uint8_t, 6> rc = {0x01, 0x42, 0x05, 0x01, 0x00, 0x01};
    auto rrc = dispatch_cmd(def, 0x0005, 1, {rc.data(), rc.size()});
    assert(rrc.any_matched);
    const char* a_rc = action_of(rrc);
    assert(a_rc && std::strcmp(a_rc, "recall_1") == 0);

    // genScenes Store (0x04): group=0x0001 LE, scene=1 -> "store_1"
    // (the gap fixed this pass).
    const std::array<std::uint8_t, 6> st = {0x01, 0x42, 0x04, 0x01, 0x00, 0x01};
    auto rst = dispatch_cmd(def, 0x0005, 1, {st.data(), st.size()});
    assert(rst.any_matched);
    const char* a_st = action_of(rst);
    assert(a_st && std::strcmp(a_st, "store_1") == 0);
}

// ── pinned INFRA gap: tint_scene (genBasic Write attr 0x4005) ─────────
// The action enum still advertises scene_<n>, but no inbound frame can
// currently produce it (Write-attr payload is not parsed). Pin that the
// advertised enum exists yet a Write-Attributes frame yields no action,
// so a future shared-parser fix flips this test deliberately.
void test_scene_gap_pinned() {
    const auto& def = devices::muller_licht::kDef_MLI_404011_MLI_404049;
    // The enum advertises scenes (z2m parity surface kept intact).
    bool advertises_scene = false;
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        const Expose& e = def.exposes[i];
        if (!e.name || std::strcmp(e.name, "action") != 0) continue;
        for (std::size_t j = 0; j < e.enum_count; ++j)
            if (e.enum_values[j] && std::strcmp(e.enum_values[j], "scene_3") == 0)
                advertises_scene = true;
    }
    assert(advertises_scene);

    // genBasic Write Attributes (cmd 0x02): attr 0x4005 LE, u8 datatype, scene=3.
    // Profile-wide, client->server (fc=0x00).
    const std::array<std::uint8_t, 7> wr = {0x00, 0x42, 0x02, 0x05, 0x40, 0x20, 0x03};
    auto rw = dispatch_cmd(def, 0x0000, 1, {wr.data(), wr.size()});
    // No converter fires today -> no "action" surfaces. Pinned.
    assert(action_of(rw) == nullptr);
}

}  // namespace

int main() {
    test_404011_colour_wheel();
    test_404022_colour_wheel();
    test_404002_recall_store();
    test_scene_gap_pinned();
    return 0;
}
