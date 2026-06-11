// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Aldi (Lightway/Müller "MEGOS") family.
//
//   Wrong bundle — 141L100RC is a "switch and dimming light remote control".
//   z2m exposes only e.action([...]) and decodes genOnOff + genLevelCtrl
//   COMMANDS into that action (fz.command_on/off/step/move/stop), with an
//   empty toZigbee (it controls nothing itself). The auto-port mis-modelled
//   it as a settable on/off `state` (kFzOnOff + kTzOnOff) — a dead control
//   whose button presses produced no `action`. Fixed to the command decoder
//   set (On/Off + Step/Move/Stop) + an `action` enum, with no to_zigbee.
//   Single endpoint -> no endpoint_map (bare `action`, no suffix).
//
// z2m-source: zigbee-herdsman-converters/src/devices/aldi.ts #141L100RC.

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

namespace zhc::devices::aldi {
extern const PreparedDefinition kDef_D141L100RC;   // MEGOS switch/dim remote
}  // namespace zhc::devices::aldi

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd,
// then payload. The cluster name must be stamped post-decode because genOnOff
// Off (cmd 0x00) and genLevelCtrl Move (cmd 0x01) share low command ids.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

// Keep `ctx` alive while the result is read: result pointers can alias scratch.
DispatchResult dispatch_zcl(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool action_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── Remote shape: action, no settable state, no write path. ────────────────
void test_remote_shape() {
    using namespace zhc::devices::aldi;
    const auto& def = kDef_D141L100RC;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));      // dead phantom control must be gone
    assert(def.to_zigbee_count == 0);        // uncontrollable
    assert(def.from_zigbee_count > 0);
    assert(!def.endpoint_action_suffix);     // single endpoint -> bare action
    // Binds both command clusters the buttons emit.
    assert(def_binds(def, 1, 0x0006));       // genOnOff
    assert(def_binds(def, 1, 0x0008));       // genLevelCtrl
}

// ── genOnOff On/Off -> action "on"/"off". ──────────────────────────────────
void test_onoff_commands() {
    using namespace zhc::devices::aldi;
    const auto& def = kDef_D141L100RC;

    RuntimeContext c1{};
    auto on = dispatch_zcl(c1, def, 0x0006, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched);
    assert(action_is(on, "action", "on"));
    assert(on.merged.find("state") == nullptr);   // no phantom relay state

    RuntimeContext c2{};
    auto off = dispatch_zcl(c2, def, 0x0006, "genOnOff", cmd_frame(0x00));
    assert(off.any_matched);
    assert(action_is(off, "action", "off"));
}

// ── genLevelCtrl Step (cmd 0x02): mode/size/transition. ────────────────────
void test_step_command() {
    using namespace zhc::devices::aldi;
    const auto& def = kDef_D141L100RC;

    // step_mode 0 (up), step_size 30, transition 5 (=0.5s, u16 LE).
    RuntimeContext c1{};
    const std::array<std::uint8_t, 4> up = {0x00, 0x1E, 0x05, 0x00};
    auto su = dispatch_zcl(c1, def, 0x0008, "genLevelCtrl",
                           cmd_frame(0x02, std::span<const std::uint8_t>(up.data(), up.size())));
    assert(su.any_matched);
    assert(action_is(su, "action", "brightness_step_up"));

    // step_mode 1 (down).
    RuntimeContext c2{};
    const std::array<std::uint8_t, 4> dn = {0x01, 0x1E, 0x05, 0x00};
    auto sd = dispatch_zcl(c2, def, 0x0008, "genLevelCtrl",
                           cmd_frame(0x02, std::span<const std::uint8_t>(dn.data(), dn.size())));
    assert(sd.any_matched);
    assert(action_is(sd, "action", "brightness_step_down"));
}

// ── genLevelCtrl Move (cmd 0x01): mode/rate. ───────────────────────────────
void test_move_command() {
    using namespace zhc::devices::aldi;
    const auto& def = kDef_D141L100RC;

    // move_mode 0 (up), rate 50.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> up = {0x00, 0x32};
    auto mu = dispatch_zcl(c1, def, 0x0008, "genLevelCtrl",
                           cmd_frame(0x01, std::span<const std::uint8_t>(up.data(), up.size())));
    assert(mu.any_matched);
    assert(action_is(mu, "action", "brightness_move_up"));

    // move_mode 1 (down).
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> dn = {0x01, 0x32};
    auto md = dispatch_zcl(c2, def, 0x0008, "genLevelCtrl",
                           cmd_frame(0x01, std::span<const std::uint8_t>(dn.data(), dn.size())));
    assert(md.any_matched);
    assert(action_is(md, "action", "brightness_move_down"));
}

// ── genLevelCtrl Stop (cmd 0x03): no payload -> "brightness_stop". ─────────
void test_stop_command() {
    using namespace zhc::devices::aldi;
    const auto& def = kDef_D141L100RC;

    RuntimeContext c{};
    auto st = dispatch_zcl(c, def, 0x0008, "genLevelCtrl", cmd_frame(0x03));
    assert(st.any_matched);
    assert(action_is(st, "action", "brightness_stop"));
}

}  // namespace

int main() {
    test_remote_shape();
    test_onoff_commands();
    test_step_command();
    test_move_command();
    test_stop_command();
    return 0;
}
