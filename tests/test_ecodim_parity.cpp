// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for EcoDim (Dutch dimmers + rotary/scene wall remotes).
// z2m-source: zigbee-herdsman-converters/src/devices/ecodim.ts.
//
// Bug fixed (multi-button remotes lost per-button identity):
//
//   ED-10011 / ED-10012 / ED-10013 / ED-10014 / ED-10015 — 2-/4-/8-button
//   wall remotes. z2m uses fz.command_on/off/move/stop + fz.battery and
//   e.action(on_<n>/off_<n>/brightness_move_up_<n>/brightness_move_down_<n>/
//   brightness_stop_<n>), distinguishing each button-pair by endpoint suffix.
//   The auto-port emitted a bare `action` (a kAlwaysGlobalKey), so every
//   button on every endpoint collapsed onto one key and the originating
//   endpoint was thrown away. Fixed by adding endpoint_map (label = the digit)
//   + endpoint_action_suffix so the dispatcher rewrites the key to action_<n>
//   per endpoint — same convention as robb ROB_200-007-0 / vesternet.
//
//   ED-10010 is the deliberate non-fix: z2m exposes action WITHOUT a suffix
//   (e.action(["on","off",...])), so the bare `action` key is CORRECT there.
//   Guard it so a future "suffix everything" sweep doesn't regress it.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <string>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ecodim {
extern const PreparedDefinition kDef_ED_10010;  // 2-button remote (no suffix)
extern const PreparedDefinition kDef_ED_10011;  // 2-button remote (action_1)
extern const PreparedDefinition kDef_ED_10012;  // 4-button remote (action_1/2)
extern const PreparedDefinition kDef_ED_10013;  // 4-button remote (action_1/2)
extern const PreparedDefinition kDef_ED_10014;  // 8-button remote (action_1..4)
extern const PreparedDefinition kDef_ED_10015;  // 8-button remote (action_1..4)
}  // namespace zhc::devices::ecodim

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
// in RuntimeContext::ep_scratch, so the result's key pointers alias `ctx`.
// The caller must keep `ctx` alive while it reads the result — hence ctx is
// passed in by reference, not created locally here.
DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn,
//    cmd, then the command payload. ───────────────────────────────────────
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

// genPowerCfg batteryPercentageRemaining (attr 0x0021) → battery%. The battery
// key is always global, so it stays bare even on suffixing devices.
void assert_battery(RuntimeContext& ctx, const PreparedDefinition& def) {
    const std::array<std::uint8_t, 1> val = {0xC8};  // 200 -> 100%
    auto r = dispatch_zcl_ep(ctx, def, 0x0001, 1, "genPowerCfg",
                             attr_report(0x0021, 0x20,
                                         std::span<const std::uint8_t>(val.data(), val.size())));
    assert(r.any_matched);
    const Value* b = r.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
}

// ─────────────────────────────────────────────────────────────────────────
// Multi-button remotes: each button keeps its identity via action_<n>; the
// bare `action` key must never leak through. `a` and `b` are two endpoints
// whose actions must land on distinct suffixed keys.
// ─────────────────────────────────────────────────────────────────────────
void check_suffixed_remote(const PreparedDefinition& def, std::uint8_t a,
                           std::uint8_t b) {
    RuntimeContext ctx{};
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map && def.endpoint_map_count > 0);
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));   // not a controllable relay
    assert(def.to_zigbee_count == 0);     // command client, never writes back

    char key_a[16], key_b[16];
    std::snprintf(key_a, sizeof(key_a), "action_%u", static_cast<unsigned>(a));
    std::snprintf(key_b, sizeof(key_b), "action_%u", static_cast<unsigned>(b));

    // genOnOff On (cmd 0x01) on endpoint a → action_<a> = "on"; bare key gone.
    auto on_a = dispatch_zcl_ep(ctx, def, 0x0006, a, "genOnOff", cmd_frame(0x01));
    assert(on_a.any_matched);
    assert(action_is(on_a, key_a, "on"));
    assert(on_a.merged.find("action") == nullptr);

    // genOnOff Off (cmd 0x00) on endpoint b → action_<b> = "off" (distinct).
    auto off_b = dispatch_zcl_ep(ctx, def, 0x0006, b, "genOnOff", cmd_frame(0x00));
    assert(off_b.any_matched);
    assert(action_is(off_b, key_b, "off"));

    // genLevelCtrl Move (cmd 0x01, mode=up/0 rate=50) on endpoint a →
    // "brightness_move_up" under suffixed key action_<a>.
    const std::array<std::uint8_t, 2> mv = {0x00, 0x32};  // mode 0 = up, rate 50
    auto mv_a = dispatch_zcl_ep(ctx, def, 0x0008, a, "genLevelCtrl",
                                cmd_frame(0x01, std::span<const std::uint8_t>(mv.data(), mv.size())));
    assert(mv_a.any_matched);
    assert(action_is(mv_a, key_a, "brightness_move_up"));

    assert_battery(ctx, def);
}

// ─────────────────────────────────────────────────────────────────────────
// ED-10010: z2m has NO action suffix, so the bare `action` key is correct.
// Regression guard against an over-eager "suffix everything" change.
// ─────────────────────────────────────────────────────────────────────────
void check_unsuffixed_remote(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    assert(!def.endpoint_action_suffix);
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);

    // genOnOff On on endpoint 1 → bare `action` = "on" (no suffix).
    auto on1 = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on1.any_matched);
    assert(action_is(on1, "action", "on"));

    assert_battery(ctx, def);
}

}  // namespace

int main() {
    using namespace zhc::devices::ecodim;

    // ED-10010: unsuffixed action is the intended shape (z2m has no _n suffix).
    check_unsuffixed_remote(kDef_ED_10010);

    // ED-10011: single button-pair on EP1, but z2m still uses the _1 suffix.
    check_suffixed_remote(kDef_ED_10011, 1, 1);

    // ED-10012 / ED-10013: two button-pairs (EP1, EP2).
    check_suffixed_remote(kDef_ED_10012, 1, 2);
    check_suffixed_remote(kDef_ED_10013, 1, 2);

    // ED-10014 / ED-10015: four button-pairs (EP1..EP4).
    check_suffixed_remote(kDef_ED_10014, 1, 4);
    check_suffixed_remote(kDef_ED_10015, 2, 3);

    std::printf("ecodim parity tests passed\n");
    return 0;
}
