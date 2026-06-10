// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for Dresden Elektronik (deCONZ / Phoscon-OEM).
// z2m-source: zigbee-herdsman-converters/src/devices/dresden_elektronik.ts.
//
// Bug fixed (scene buttons silent on the BN-600085 remote):
//
//   BN-600085 ("Scene Switch") is a 3-part battery scene remote — a
//   genOnOff / genLevelCtrl / lightingColorCtrl / genScenes CLIENT that only
//   SENDS commands. z2m wires m.commandsScenes(), which decodes genScenes
//   (0x0005) commandRecall → action "recall_<scene>" and commandStore →
//   "store_<scene>". The port wired OnOff/LevelCtrl/ColorCtrl + battery but
//   DROPPED the scene decoders, with a stale comment claiming "no generic
//   decoder yet" — false: kFzCommandRecall + kFzCommandStore already live in
//   _generic/_shared. So every scene-button press was silent. Fixed by wiring
//   both converters. The remote is single-endpoint, so actions stay bare.
//
// Bug fixed (BN-600087 collapsed both rockers onto one key):
//
//   BN-600087 ("Lighting Switch") is a 2-endpoint command remote. z2m wires
//   m.commandsOnOff/LevelCtrl/ColorCtrl(["1","2"]), which postfix the action
//   per endpoint (on_1 / on_2 …). The port had the endpoint_map but NOT
//   endpoint_action_suffix — and `action` is a kAlwaysGlobalKey, so the
//   dispatcher left it bare and both buttons collapsed onto one `action`
//   (a 2-button remote was indistinguishable from 1). Set
//   endpoint_action_suffix so the key is rewritten action_1 / action_2.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::dresden_elektronik {
extern const PreparedDefinition kDef_BN_600085;  // 3-part scene remote (single EP)
extern const PreparedDefinition kDef_BN_600087;  // 2-part light switch (2 EP)
}  // namespace zhc::devices::dresden_elektronik

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

// NOTE: multi-endpoint key suffixing allocates the rewritten `<key>_<label>`
// in RuntimeContext::ep_scratch, so the result's key pointers alias `ctx`;
// keep `ctx` alive while reading the result.
DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;  // stamp post-decode (low cmd ids collide otherwise)
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd, payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ─────────────────────────────────────────────────────────────────────────
// Bug fix: BN-600085 scene remote surfaces genScenes recall/store as bare
// `action` (single-endpoint, no suffixing), alongside the OnOff/LevelCtrl
// command stream it already decoded.
// ─────────────────────────────────────────────────────────────────────────
void check_bn_600085_scene_remote(const PreparedDefinition& def) {
    RuntimeContext ctx{};

    // Command client: exposes action, no controllable state, no Tz, no
    // per-endpoint suffixing (3-part remote binds EP1 only).
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(!def.endpoint_action_suffix);
    assert(def.endpoint_map == nullptr || def.endpoint_map_count == 0);

    // genScenes Recall (cmd 0x05, payload group u16 LE + scene u8) on EP1.
    // Recall scene 2 from group 1 → action = "recall_2".
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x02};  // group=1, scene=2
    auto recall = dispatch_zcl_ep(ctx, def, 0x0005, 1, "genScenes",
                                  cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(recall.any_matched);
    assert(action_is(recall, "action", "recall_2"));

    // genScenes Store (cmd 0x04, payload group u16 LE + scene u8) on EP1.
    // Store scene 5 from group 1 → action = "store_5".
    const std::array<std::uint8_t, 3> st = {0x01, 0x00, 0x05};  // group=1, scene=5
    auto store = dispatch_zcl_ep(ctx, def, 0x0005, 1, "genScenes",
                                 cmd_frame(0x04, std::span<const std::uint8_t>(st.data(), st.size())));
    assert(store.any_matched);
    assert(action_is(store, "action", "store_5"));

    // Regression: the pre-existing OnOff command stream still decodes.
    auto on = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched);
    assert(action_is(on, "action", "on"));

    // Regression: genLevelCtrl Move (cmd 0x01, mode=up/0 rate=50) → "brightness_move_up".
    const std::array<std::uint8_t, 2> mv = {0x00, 0x32};
    auto move = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                                cmd_frame(0x01, std::span<const std::uint8_t>(mv.data(), mv.size())));
    assert(move.any_matched);
    assert(action_is(move, "action", "brightness_move_up"));
}

// ─────────────────────────────────────────────────────────────────────────
// Bug fix: BN-600087 is a 2-endpoint command remote. z2m wires
// m.commandsOnOff/LevelCtrl/ColorCtrl(["1","2"]) (NO scenes); each rocker must
// keep identity (action_1 / action_2). Requires BOTH endpoint_map AND
// endpoint_action_suffix (action is a kAlwaysGlobalKey).
// ─────────────────────────────────────────────────────────────────────────
void check_bn_600087_two_button(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(def.endpoint_map != nullptr && def.endpoint_map_count == 2);
    assert(def.endpoint_action_suffix);

    // genOnOff On (cmd 0x01) on EP1 → action_1 = "on"; bare key gone.
    auto on1 = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on1.any_matched);
    assert(action_is(on1, "action_1", "on"));
    assert(on1.merged.find("action") == nullptr);

    // genOnOff Off (cmd 0x00) on EP2 → action_2 = "off" (distinct key).
    auto off2 = dispatch_zcl_ep(ctx, def, 0x0006, 2, "genOnOff", cmd_frame(0x00));
    assert(off2.any_matched);
    assert(action_is(off2, "action_2", "off"));
}

}  // namespace

int main() {
    using namespace zhc::devices::dresden_elektronik;

    check_bn_600085_scene_remote(kDef_BN_600085);
    check_bn_600087_two_button(kDef_BN_600087);

    std::printf("dresden_elektronik parity tests passed\n");
    return 0;
}
