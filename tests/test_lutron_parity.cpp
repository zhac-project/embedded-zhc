// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Lutron remote family.
//
// Lutron's two ZHA entries are both battery remotes that drive a paired bulb
// by emitting genLevelCtrl cluster commands — neither has any on/off relay:
//
//   * LZL4BWHL01 (Connected Bulb Remote): z2m fromZigbee
//       [fz.command_step, fz.command_step, custom_command_move_to_level,
//        fz.command_stop], toZigbee [], exposes action([brightness_step_down,
//        brightness_step_up, brightness_stop, brightness_move_to_level]).
//   * Z3-1BRL (Aurora rotary dimmer): z2m fromZigbee
//       [custom_command_move_to_level], m.battery(), toZigbee [], exposes
//       action([brightness]) + numeric(brightness), configure binds
//       genLevelCtrl.
//
// The auto-generator wrong-bundled BOTH as settable on/off lights (kFzOnOff +
// kTzOnOff + a writable `state`) and dropped the `action` decode — every
// rotary turn / button press was dead and the device falsely advertised a
// relay. Fix (mirrors the paulmann remote fixes): graduate each def, swap the
// on/off fz/tz for the genLevelCtrl command decoders (kFzCommandStep /
// kFzCommandMoveToLevel / kFzCommandStop), expose `action`, drop the phantom
// state + toolbox. Both are single-endpoint (z2m has no deviceEndpoints) so
// each emits a bare `action`. Z3-1BRL keeps battery (a global key).
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * every remote exposes `action` and NO settable `state`,
//   * every remote has an empty toolbox (to_zigbee_count == 0),
//   * genLevelCtrl Step / MoveToLevel / Stop decode to the right `action`,
//   * single-endpoint → bare `action` (never action_<n>),
//   * Z3-1BRL battery stays a global key.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lutron.ts
//             #LZL4BWHL01 / #Z3-1BRL +
//             converters/fromZigbee.ts command_step / command_move_to_level /
//             command_stop.

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

namespace zhc::devices::lutron {
extern const PreparedDefinition kDef_LZL4BWHL01;
extern const PreparedDefinition kDef_Z3_1BRL;
}  // namespace zhc::devices::lutron

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

// Shared invariants for every remote: action expose, NO settable state,
// empty toolbox (the device cannot be controlled), single endpoint.
void check_remote_shape(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));   // phantom relay must be gone
    assert(def.to_zigbee_count == 0);     // toZigbee: [] — no write path
    assert(def.from_zigbee_count > 0);
    assert(!def.endpoint_action_suffix);  // single endpoint → bare action
    assert(def.endpoint_map == nullptr);
}

// ── LZL4BWHL01: 4-button remote → bare action over genLevelCtrl. ──────────
void check_LZL4BWHL01(const PreparedDefinition& def) {
    check_remote_shape(def);

    RuntimeContext ctx{};

    // genLevelCtrl Step up (cmd 0x02): mode=0, size=10, tt=0 →
    // action = "brightness_step_up".
    const std::array<std::uint8_t, 4> step_up = {0x00, 0x0A, 0x00, 0x00};
    auto su = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x02, std::span<const std::uint8_t>(step_up.data(), step_up.size())));
    assert(su.any_matched);
    assert(action_is(su, "action", "brightness_step_up"));
    assert(su.merged.find("action_1") == nullptr);

    // genLevelCtrl Step down (cmd 0x02): mode=1 → action = "brightness_step_down".
    const std::array<std::uint8_t, 4> step_dn = {0x01, 0x0A, 0x00, 0x00};
    auto sd = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x02, std::span<const std::uint8_t>(step_dn.data(), step_dn.size())));
    assert(sd.any_matched);
    assert(action_is(sd, "action", "brightness_step_down"));

    // genLevelCtrl MoveToLevel (cmd 0x00): level=128, tt=0 →
    // action = "brightness_move_to_level" + level=128.
    const std::array<std::uint8_t, 3> mtl = {0x80, 0x00, 0x00};
    auto ml = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x00, std::span<const std::uint8_t>(mtl.data(), mtl.size())));
    assert(ml.any_matched);
    assert(action_is(ml, "action", "brightness_move_to_level"));
    const Value* lvl = ml.merged.find("level");
    assert(lvl && lvl->type == ValueType::Uint && lvl->u == 128);

    // genLevelCtrl Stop (cmd 0x03) → action = "brightness_stop".
    auto st = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl", cmd_frame(0x03));
    assert(st.any_matched);
    assert(action_is(st, "action", "brightness_stop"));

    // The phantom on/off relay is gone: genOnOff On must NOT decode to a state.
    RuntimeContext ctx2{};
    auto on = dispatch_zcl_ep(ctx2, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.merged.find("state") == nullptr);

    assert(def_binds(def, 1, 0x0008));   // genLevelCtrl bound
}

// ── Z3-1BRL: rotary dimmer → bare action + battery. ───────────────────────
void check_Z3_1BRL(const PreparedDefinition& def) {
    check_remote_shape(def);
    assert(def_exposes(def, "battery"));

    RuntimeContext ctx{};

    // genLevelCtrl MoveToLevel (cmd 0x00): level=64 → action + level=64.
    const std::array<std::uint8_t, 3> mtl = {0x40, 0x00, 0x00};
    auto ml = dispatch_zcl_ep(ctx, def, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x00, std::span<const std::uint8_t>(mtl.data(), mtl.size())));
    assert(ml.any_matched);
    assert(action_is(ml, "action", "brightness_move_to_level"));
    const Value* lvl = ml.merged.find("level");
    assert(lvl && lvl->type == ValueType::Uint && lvl->u == 64);

    // Battery (genPowerCfg attr 0x0021 = 200 → 100%) stays a global key.
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 1> bv = {0xC8};
    auto br = dispatch_zcl_ep(ctx2, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20,
                                          std::span<const std::uint8_t>(bv.data(), bv.size())));
    assert(br.any_matched);
    const Value* b = br.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);

    assert(def_binds(def, 1, 0x0008));   // genLevelCtrl bound (z2m configure)
    assert(def_binds(def, 1, 0x0001));   // genPowerCfg bound (battery)
}

}  // namespace

int main() {
    using namespace zhc::devices::lutron;
    check_LZL4BWHL01(kDef_LZL4BWHL01);
    check_Z3_1BRL(kDef_Z3_1BRL);
    return 0;
}
