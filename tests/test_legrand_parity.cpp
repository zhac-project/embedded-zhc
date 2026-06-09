// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Legrand / BTicino / Netatmo battery action
// remotes. z2m-source: zigbee-herdsman-converters/src/devices/legrand.ts
// + lib/legrand.ts.
//
// Wrong device class (button stubbed as on/off switch): 067694, 067773,
// 067774, WNAL63, 067646, 067755 and 067767 all shipped the generated
// kFzOnOff + a controllable on/off TZ. None of these devices has a local
// relay — they are wireless wall pushbuttons / scene remotes that only
// *send* cluster commands (genOnOff / genLevelCtrl / genScenes /
// closuresWindowCovering, client->server). kFzOnOff decodes attribute
// *reports*, so it never fired: the "state" expose was dead and the TZ
// meaningless. Each graduated to a Tier-2 parent wiring the generic
// command-action decoders and exposing "action" + "battery" instead of a
// controllable "state".
//
// These tests pin the corrected wire-shape decode for a representative
// device of each command family (on/off, brightness move, dual-gang
// suffix, cover, scene recall).

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

namespace zhc::devices::legrand {
extern const PreparedDefinition kDef_D067694;   // toggle remote (on/off/toggle)
extern const PreparedDefinition kDef_D067773;   // remote (+ brightness move)
extern const PreparedDefinition kDef_D067774;   // dual-gang (suffixed action)
extern const PreparedDefinition kDef_WNAL63;    // Netatmo remote dimmer
extern const PreparedDefinition kDef_D067646;   // shutter remote (cover cmds)
extern const PreparedDefinition kDef_D067755;   // 4-scene recall remote
extern const PreparedDefinition kDef_D067767;   // color-dimmer remote
}  // namespace zhc::devices::legrand

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

// ── 067694 — toggle pushbutton → action "toggle", no switch ──────────
void test_067694_toggle() {
    const auto& def = devices::legrand::kDef_D067694;
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "state"));        // not a controllable switch
    assert(def.to_zigbee_count == 0);          // no on/off TZ

    // genOnOff cmd 0x02 (Toggle) -> action "toggle".
    const std::array<std::uint8_t, 3> f = {0x01, 0x42, 0x02};
    auto r = dispatch_cmd(def, 0x0006, 1, {f.data(), f.size()});
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "toggle") == 0);
    assert(r.merged.find("state") == nullptr); // raw state never published
}

// ── 067773 — remote: on/off + brightness move ───────────────────────
void test_067773_actions() {
    const auto& def = devices::legrand::kDef_D067773;
    assert(def_exposes(def, "action") && !def_exposes(def, "state"));
    assert(def_binds(def, 0x0008));            // genLevelCtrl bound

    // genOnOff On (0x01) -> "on".
    const std::array<std::uint8_t, 3> on = {0x01, 0x42, 0x01};
    auto r = dispatch_cmd(def, 0x0006, 1, {on.data(), on.size()});
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "on") == 0);

    // genLevelCtrl Move (cmd 0x01) mode=0 (up), rate=50 -> "brightness_move_up".
    const std::array<std::uint8_t, 5> mv = {0x01, 0x42, 0x01, 0x00, 0x32};
    auto r2 = dispatch_cmd(def, 0x0008, 1, {mv.data(), mv.size()});
    assert(r2.any_matched);
    const char* a2 = action_of(r2);
    assert(a2 && std::strcmp(a2, "brightness_move_up") == 0);
}

// ── 067774 — dual-gang: action suffixed per endpoint ─────────────────
void test_067774_endpoint_suffix() {
    const auto& def = devices::legrand::kDef_D067774;
    assert(def.endpoint_map_count == 2);
    assert(def.endpoint_action_suffix);
    assert(!def_exposes(def, "state"));

    // genOnOff On on endpoint 1 (left) -> action_left = "on".
    const std::array<std::uint8_t, 3> on = {0x01, 0x42, 0x01};
    auto rl = dispatch_cmd(def, 0x0006, 1, {on.data(), on.size()});
    assert(rl.any_matched);
    const Value* al = rl.merged.find("action_left");
    assert(al && al->type == ValueType::StringRef && std::strcmp(al->str, "on") == 0);
    assert(rl.merged.find("action") == nullptr);   // bare key suffixed away

    // genOnOff On on endpoint 2 (right) -> action_right = "on".
    auto rr = dispatch_cmd(def, 0x0006, 2, {on.data(), on.size()});
    assert(rr.any_matched);
    const Value* ar = rr.merged.find("action_right");
    assert(ar && ar->type == ValueType::StringRef && std::strcmp(ar->str, "on") == 0);
}

// ── WNAL63 — Netatmo remote dimmer: same command family ──────────────
void test_wnal63_actions() {
    const auto& def = devices::legrand::kDef_WNAL63;
    assert(def_exposes(def, "action") && !def_exposes(def, "state"));
    const std::array<std::uint8_t, 3> off = {0x01, 0x42, 0x00};
    auto r = dispatch_cmd(def, 0x0006, 1, {off.data(), off.size()});
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "off") == 0);
}

// ── 067646 — shutter remote: cover commands → open/close/stop ────────
void test_067646_cover_action() {
    const auto& def = devices::legrand::kDef_D067646;
    assert(def_exposes(def, "action") && !def_exposes(def, "state"));
    assert(def_binds(def, 0x0102));            // closuresWindowCovering bound
    assert(def.to_zigbee_count == 0);

    // closuresWindowCovering upOpen (cmd 0x00) -> "open".
    const std::array<std::uint8_t, 3> op = {0x01, 0x42, 0x00};
    auto r = dispatch_cmd(def, 0x0102, 1, {op.data(), op.size()});
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "open") == 0);

    // downClose (cmd 0x01) -> "close".
    const std::array<std::uint8_t, 3> cl = {0x01, 0x42, 0x01};
    auto r2 = dispatch_cmd(def, 0x0102, 1, {cl.data(), cl.size()});
    assert(r2.any_matched);
    const char* a2 = action_of(r2);
    assert(a2 && std::strcmp(a2, "close") == 0);
}

// ── 067755 — scene remote: genScenes recall → recall_<n> ─────────────
void test_067755_recall() {
    const auto& def = devices::legrand::kDef_D067755;
    assert(def_exposes(def, "action") && !def_exposes(def, "state"));
    assert(def_binds(def, 0x0005));            // genScenes bound

    // genScenes Recall (cmd 0x05): group=0x0001 LE, scene=1 -> "recall_1".
    const std::array<std::uint8_t, 6> rc = {0x01, 0x42, 0x05, 0x01, 0x00, 0x01};
    auto r = dispatch_cmd(def, 0x0005, 1, {rc.data(), rc.size()});
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "recall_1") == 0);
}

// ── 067767 — color-dimmer remote: brightness move surfaces as action ─
void test_067767_actions() {
    const auto& def = devices::legrand::kDef_D067767;
    assert(def_exposes(def, "action") && !def_exposes(def, "state"));

    // genLevelCtrl Move (cmd 0x01) mode=1 (down), rate=80 -> "brightness_move_down".
    const std::array<std::uint8_t, 5> mv = {0x01, 0x42, 0x01, 0x01, 0x50};
    auto r = dispatch_cmd(def, 0x0008, 1, {mv.data(), mv.size()});
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "brightness_move_down") == 0);
}

}  // namespace

int main() {
    test_067694_toggle();
    test_067773_actions();
    test_067774_endpoint_suffix();
    test_wnal63_actions();
    test_067646_cover_action();
    test_067755_recall();
    test_067767_actions();
    return 0;
}
