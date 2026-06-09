// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for Vesternet (rebranded Sunricher hardware).
// z2m-source: zigbee-herdsman-converters/src/devices/vesternet.ts.
//
// Bug fixed (lost per-button identity on the multi-endpoint remotes):
//
//   VES-ZB-REM-013 (12-button) / VES-ZB-WAL-011 (4-button) / VES-ZB-WAL-012
//   (8-button) are battery-powered scene remotes — genOnOff/genLevelCtrl/
//   genScenes CLIENTS that only SEND commands across endpoints 1..N. The
//   command decoders emit a bare `action` ("on", "brightness_move_up",
//   "recall_<scene>", …), which is a kAlwaysGlobalKey, so without
//   endpoint_action_suffix every rocker group collapsed onto the same
//   `action` key and the originating endpoint was discarded — a 4/8/12-button
//   remote was indistinguishable from a single button. z2m instead
//   distinguishes them per endpoint (on_1..on_4, …, recall_1_1..recall_2_4).
//   Fixed by adding the per-endpoint label map ({1,2,3,4} / {1,2}) and setting
//   endpoint_action_suffix so the dispatcher rewrites the key to `action_<n>`.
//   Same convention as the robb/sunricher rebrand of this exact hardware
//   (Rob_ROB_200_007_0 = ZG2833K8_EU05, Rob_ROB_200_008_0 = ZG2833K4_EU06).
//
// Regression guards (already-correct defs that must stay correct):
//   * VES-ZB-WAL-006 — single-EP 2-button remote: bare `action`, NO
//     endpoint_map (mirrors the robb ROB_200-009-0 single-EP precedent).
//   * VES-ZB-SWI-015 — 2-channel switch: state_l1/state_l2 via endpoint_map.
//   * VES-ZB-PIR-21 — IAS occupancy sensor: `occupancy` decodes from zone bit.

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

namespace zhc::devices::vesternet {
extern const PreparedDefinition kDef_VES_ZB_REM_013;  // 12-button remote
extern const PreparedDefinition kDef_VES_ZB_WAL_011;  // 4-button wall controller
extern const PreparedDefinition kDef_VES_ZB_WAL_012;  // 8-button wall controller
extern const PreparedDefinition kDef_VES_ZB_WAL_006;  // 2-button wall controller (single EP)
extern const PreparedDefinition kDef_VES_ZB_SWI_015;  // 2-channel switch
extern const PreparedDefinition kDef_VES_ZB_PIR_21;   // IAS occupancy sensor
}  // namespace zhc::devices::vesternet

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
// The caller must keep `ctx` alive for as long as it reads the result.
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

// ── genPowerCfg batteryPercentageRemaining (attr 0x0021) → battery%. ──────
// REM-013 reports battery on EP1; the value is a kAlwaysGlobalKey so it is
// NEVER suffixed even though the device opts into endpoint_action_suffix.
void assert_battery_global(RuntimeContext& ctx, const PreparedDefinition& def) {
    const std::array<std::uint8_t, 1> val = {0xC8};  // 200 -> 100%
    auto r = dispatch_zcl_ep(ctx, def, 0x0001, 1, "genPowerCfg",
                             attr_report(0x0021, 0x20,
                                         std::span<const std::uint8_t>(val.data(), val.size())));
    assert(r.any_matched);
    const Value* b = r.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
    // battery must NOT be suffixed even with endpoint_action_suffix set.
    assert(r.merged.find("battery_1") == nullptr);
}

// ─────────────────────────────────────────────────────────────────────────
// Multi-button remotes keep per-button identity via endpoint_action_suffix →
// action_<n>, instead of collapsing every button onto a bare `action`.
// `a` and `b` are two distinct source endpoints to probe.
// ─────────────────────────────────────────────────────────────────────────
void check_multibutton_remote(const PreparedDefinition& def, std::uint8_t a,
                              std::uint8_t b) {
    RuntimeContext ctx{};

    // Regression guard: no controllable `state`, exposes `action`, writes
    // back nothing (command client), and opts into per-button suffixing.
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map != nullptr && def.endpoint_map_count > 0);

    char key_a[16], key_b[16];
    std::snprintf(key_a, sizeof(key_a), "action_%u", static_cast<unsigned>(a));
    std::snprintf(key_b, sizeof(key_b), "action_%u", static_cast<unsigned>(b));

    // genOnOff On (cmd 0x01) on endpoint a → action_<a> = "on"; bare key gone.
    auto on_a = dispatch_zcl_ep(ctx, def, 0x0006, a, "genOnOff", cmd_frame(0x01));
    assert(on_a.any_matched);
    assert(action_is(on_a, key_a, "on"));
    assert(on_a.merged.find("action") == nullptr);

    // genOnOff Off (cmd 0x00) on endpoint b → action_<b> = "off" (distinct key).
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

    assert_battery_global(ctx, def);
}

// ─────────────────────────────────────────────────────────────────────────
// REM-013 additionally decodes genScenes Recall → recall_<scene>, with the
// endpoint preserved in the suffixed key. (genScenes Recall cmd 0x05,
// payload: group u16 LE, scene u8.)
// ─────────────────────────────────────────────────────────────────────────
void check_rem_013_recall(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    // Recall scene 3 from group 1 on endpoint 2 → action_2 = "recall_3".
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x03};  // group=1, scene=3
    auto r = dispatch_zcl_ep(ctx, def, 0x0005, 2, "genScenes",
                             cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(r.any_matched);
    assert(action_is(r, "action_2", "recall_3"));
    assert(r.merged.find("action") == nullptr);
}

// ─────────────────────────────────────────────────────────────────────────
// Regression: WAL-006 is a single-endpoint 2-button remote (z2m binds EP1
// only, all actions land on `_1`). It carries NO endpoint_map, so `action`
// stays bare — matching the robb ROB_200-009-0 single-EP precedent.
// ─────────────────────────────────────────────────────────────────────────
void check_single_ep_remote(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(!def.endpoint_action_suffix);
    assert(def.endpoint_map == nullptr || def.endpoint_map_count == 0);

    auto on = dispatch_zcl_ep(ctx, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched);
    assert(action_is(on, "action", "on"));  // bare, unsuffixed
}

// ─────────────────────────────────────────────────────────────────────────
// Regression: SWI-015 2-channel switch demuxes genOnOff per endpoint into
// state_l1 / state_l2 via its endpoint_map.
// ─────────────────────────────────────────────────────────────────────────
void check_two_channel_switch(const PreparedDefinition& def) {
    RuntimeContext ctx{};
    assert(def_exposes(def, "state_l1"));
    assert(def_exposes(def, "state_l2"));
    assert(def.endpoint_map != nullptr && def.endpoint_map_count == 2);
    assert(!def.endpoint_action_suffix);  // this device suffixes via labels, not action

    // genOnOff onOff attr (0x0000, bool) = 1 on EP2 → state_l2 = true.
    const std::array<std::uint8_t, 1> on = {0x01};
    auto r2 = dispatch_zcl_ep(ctx, def, 0x0006, 2, "genOnOff",
                              attr_report(0x0000, 0x10,
                                          std::span<const std::uint8_t>(on.data(), on.size())));
    assert(r2.any_matched);
    const Value* s2 = r2.merged.find("state_l2");
    assert(s2 && s2->type == ValueType::Bool && s2->b);
    // The unsuffixed `state` key must not leak.
    assert(r2.merged.find("state") == nullptr);
}

// ─────────────────────────────────────────────────────────────────────────
// Regression: PIR-21 is an IAS occupancy sensor (battery + iasZoneAlarm). It
// wires kFzIasZone, which decodes the ZoneStatus attribute (0x0002) into
// occupancy/alarm_1/alarm_2/tamper/battery_low. IAS enrollment is global and
// the live decode path is enrollment-gated, so — mirroring the robb PIR
// precedent — we assert the wiring (exposes present, read-only) rather than
// re-dispatch an enrollment-dependent notification here.
// ─────────────────────────────────────────────────────────────────────────
void check_pir_occupancy(const PreparedDefinition& def) {
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "battery"));
    assert(def.to_zigbee_count == 0);
    // PIR-21 is single-endpoint: no per-EP suffixing.
    assert(!def.endpoint_action_suffix);
    assert(def.endpoint_map == nullptr || def.endpoint_map_count == 0);
}

}  // namespace

int main() {
    using namespace zhc::devices::vesternet;

    // Bug fix — multi-button remotes keep per-button identity (action_<n>).
    check_multibutton_remote(kDef_VES_ZB_WAL_011, 1, 2);
    check_multibutton_remote(kDef_VES_ZB_WAL_012, 1, 4);
    check_multibutton_remote(kDef_VES_ZB_REM_013, 2, 3);
    check_rem_013_recall(kDef_VES_ZB_REM_013);

    // Regression guards — already-correct defs.
    check_single_ep_remote(kDef_VES_ZB_WAL_006);
    check_two_channel_switch(kDef_VES_ZB_SWI_015);
    check_pir_occupancy(kDef_VES_ZB_PIR_21);

    std::printf("vesternet parity tests passed\n");
    return 0;
}
