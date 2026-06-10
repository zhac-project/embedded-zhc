// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for The Light Group (SLC SmartOne) remotes.
//
// TLG is overwhelmingly a lighting vendor (m.light(...) drivers/dimmers) —
// those ports are CLEAN against z2m and exercised by the generic suites. The
// real z2m<->ZHC gaps were the two *remotes*: S57003 (4-channel wall remote)
// and S57007 (3-button remote control). Both have `toZigbee: []` (they cannot
// be controlled), yet the auto-generator wrong-bundled each as a settable
// on/off light — kFzOnOff + kTzOnOff + a writable `state` expose — and dropped
// the `action` decode entirely (S57003 also dropped the endpoint_map, so the
// four rockers would collide on one key).
//
// Fix (mirrors paulmann 501.37 / 501.40): graduate each def, swap on/off
// fz/tz for the genOnOff / genLevelCtrl / genScenes command decoders, expose
// `action`, drop the phantom state + toolbox, and add endpoint_map +
// endpoint_action_suffix so each button keeps its identity (action_<n>).
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * each remote exposes `action` and NO settable `state`,
//   * each remote has an empty toolbox (to_zigbee_count == 0),
//   * genOnOff / genLevelCtrl / genScenes commands decode to the right
//     per-endpoint `action_<n>` labels (never a bare `action`),
//   * battery stays a global key (never suffixed).
//
// z2m-source: zigbee-herdsman-converters/src/devices/the_light_group.ts
//             #S57003 / #S57007 + converters/fromZigbee.ts command_* +
//             lib/modernExtend.ts commandsOnOff / commandsScenes.

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

namespace zhc::devices::the_light_group {
extern const PreparedDefinition kDef_S57003;
extern const PreparedDefinition kDef_S57007;
}  // namespace zhc::devices::the_light_group

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

// NOTE: multi-endpoint key suffixing allocates the rewritten `<key>_<label>`
// in RuntimeContext scratch, so result key pointers alias `ctx`. Keep `ctx`
// alive for as long as the result is read.
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

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd,
// then the command payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

// ZCL attribute report builder (fc=0x18, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── Shared invariants for every remote: action expose, NO settable state,
//    empty toolbox (the device cannot be controlled). ──────────────────────
void check_remote_shape(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));   // phantom relay must be gone
    assert(def.to_zigbee_count == 0);     // toZigbee: [] — no write path
    assert(def.from_zigbee_count > 0);
    assert(def.endpoint_action_suffix);
}

// ── S57003: 4-channel wall remote → action_1..action_4. ───────────────────
void check_s57003(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(def.endpoint_map && def.endpoint_map_count == 4);

    // On press on EP1 → action_1 = "on", no bare `action`.
    RuntimeContext ctx1{};
    auto on1 = dispatch_zcl_ep(ctx1, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on1.any_matched);
    assert(action_is(on1, "action_1", "on"));
    assert(on1.merged.find("action") == nullptr);

    // Off press on EP4 → action_4 = "off".
    RuntimeContext ctx2{};
    auto off4 = dispatch_zcl_ep(ctx2, def, 0x0006, 4, "genOnOff", cmd_frame(0x00));
    assert(off4.any_matched);
    assert(action_is(off4, "action_4", "off"));
    assert(off4.merged.find("action") == nullptr);

    // genLevelCtrl Move up on EP3 (cmd 0x01, move_mode=0, rate=50) →
    // action_3 = "brightness_move_up".
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 2> mv = {0x00, 0x32};
    auto mvr = dispatch_zcl_ep(ctx3, def, 0x0008, 3, "genLevelCtrl",
                               cmd_frame(0x01, std::span<const std::uint8_t>(mv.data(), mv.size())));
    assert(mvr.any_matched);
    assert(action_is(mvr, "action_3", "brightness_move_up"));

    // genLevelCtrl Stop on EP2 (cmd 0x03) → action_2 = "brightness_stop".
    RuntimeContext ctx4{};
    auto st = dispatch_zcl_ep(ctx4, def, 0x0008, 2, "genLevelCtrl", cmd_frame(0x03));
    assert(st.any_matched);
    assert(action_is(st, "action_2", "brightness_stop"));

    // Battery (genPowerCfg attr 0x0021 = 200 → 100%) stays global on EP1.
    RuntimeContext ctx5{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx5, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    const Value* b = br.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
    assert(br.merged.find("battery_1") == nullptr);   // battery never suffixed

    // Binds: genOnOff + genLevelCtrl on all four endpoints.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 4, 0x0006));
    assert(def_binds(def, 4, 0x0008));
}

// ── S57007: 3-button remote (commandsOnOff + commandsScenes) →
//    action_1..action_3. ─────────────────────────────────────────────────
void check_s57007(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(def.endpoint_map && def.endpoint_map_count == 3);

    // Off press on EP1 → action_1 = "off".
    RuntimeContext ctx1{};
    auto off1 = dispatch_zcl_ep(ctx1, def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
    assert(off1.any_matched);
    assert(action_is(off1, "action_1", "off"));
    assert(off1.merged.find("action") == nullptr);

    // genScenes Recall (cmd 0x05): group u16 + scene u8=2 on EP2 →
    // action_2 = "recall_2".
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 3> rc = {0x01, 0x00, 0x02};
    auto re = dispatch_zcl_ep(ctx2, def, 0x0005, 2, "genScenes",
                              cmd_frame(0x05, std::span<const std::uint8_t>(rc.data(), rc.size())));
    assert(re.any_matched);
    assert(action_is(re, "action_2", "recall_2"));
    assert(re.merged.find("action") == nullptr);

    // genScenes Store (cmd 0x04): group u16 + scene u8=1 on EP3 →
    // action_3 = "store_1".
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 3> sc = {0x01, 0x00, 0x01};
    auto so = dispatch_zcl_ep(ctx3, def, 0x0005, 3, "genScenes",
                              cmd_frame(0x04, std::span<const std::uint8_t>(sc.data(), sc.size())));
    assert(so.any_matched);
    assert(action_is(so, "action_3", "store_1"));

    // Battery stays global on EP1.
    RuntimeContext ctx4{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx4, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    const Value* b = br.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);

    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 2, 0x0005));   // genScenes bound on EP2
    assert(def_binds(def, 3, 0x0005));
}

}  // namespace

int main() {
    using namespace zhc::devices::the_light_group;
    check_s57003(kDef_S57003);
    check_s57007(kDef_S57007);
    return 0;
}
