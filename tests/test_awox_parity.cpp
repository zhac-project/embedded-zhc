// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the AwoX / EGLO non-light family. z2m-source:
// zigbee-herdsman-converters/src/devices/awox.ts.
//
// Three generated defs lowered a sensor / remote onto a dead on/off
// switch surface:
//
//   * EPIR_Zm — Connect-Z PIR motion sensor. z2m = m.battery() +
//     m.occupancy() + m.commandsOnOff() + m.commandsLevelCtrl(). The
//     generated def wired kFzOnOff + kFzBattery and exposed
//     state/battery/voltage — `occupancy` (the device's whole purpose)
//     was dropped, so the msOccupancySensing report was dead. Graduated
//     to a Tier-2 parent wiring kFzOccupancy + an `occupancy` expose +
//     0x0406 binding.
//
//   * ERCU_WS_Zm — Connect-Z wall-mount light remote (multiEndpoint
//     {"1":1,"3":3}). z2m = m.commandsOnOff/LevelCtrl/ColorCtrl, i.e. a
//     pure command transmitter exposing `action`. The generated def
//     wired kFzOnOff + an on/off TZ (both dead) and exposed `state`.
//     Re-wired to the generic command-action decoders; endpoint_map +
//     action suffix split the banks as action_1 / action_3.
//
//   * 33952 (ERCU_Zm) — battery remote controller. Same wrong-class
//     stub (kFzOnOff + on/off TZ + dead `state`). Re-wired to the
//     generic command-action decoders (on/off/step/move/stop/recall/
//     step_color_temp). The AwoX-specific raw colour/refresh/scenes
//     decoders have no generic equivalent and are intentionally deferred.
//
// These tests pin the corrected decode: occupancy from the PIR, the
// per-endpoint action suffix on the wall remote, and the command-action
// stream on both remotes.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::awox {
extern const PreparedDefinition kDef_EPIR_Zm;      // PIR motion sensor
extern const PreparedDefinition kDef_ERCU_WS_Zm;   // wall-mount remote (multi-EP)
extern const PreparedDefinition kDef_D33952;       // remote controller (ERCU_Zm)
}  // namespace zhc::devices::awox

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

const char* str_of(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

// Dispatch a Report-Attributes frame (server->client, fc=0x18). The
// decoder leaves `cluster` numeric; the named-cluster converters match on
// the string, so the caller passes the cluster name to stamp post-decode
// (mirrors test_ctm_parity's occupancy path).
//   bytes = ZCL payload: fc(1)=0x18 | tsn(1) | cmd(1)=0x0A | <attr recs...>
DispatchResult dispatch_report(const PreparedDefinition& def, std::uint16_t cluster_id,
                               const char* cluster_name, std::uint8_t src_ep,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Dispatch a cluster-specific command frame (client->server, fc=0x01).
// The decoder leaves `cluster` numeric; the named-cluster command
// converters match on the string (and cmd-id 0x00 collides between
// genOnOff Off and genLevelCtrl MoveToLevel), so the caller stamps the
// cluster name post-decode — exactly as the radio layer supplies it in
// production.
//   bytes = ZCL payload: fc(1)=0x01 | tsn(1) | cmd(1) | body...
DispatchResult dispatch_cmd(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── EPIR_Zm — PIR motion sensor decodes occupancy ────────────────────
void test_epir_occupancy() {
    const auto& def = devices::awox::kDef_EPIR_Zm;
    assert(def_exposes(def, "occupancy"));     // primary signal restored
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 0x0406));            // msOccupancySensing bound

    // Report Attributes: fc=0x18 | tsn | cmd=0x0A | attr 0x0000 LE |
    // type 0x18 (bitmap8) | value. bit0=1 -> occupied.
    const std::array<std::uint8_t, 7> rep = {0x18, 0x10, 0x0A, 0x00, 0x00, 0x18, 0x01};
    auto r = dispatch_report(def, 0x0406, "msOccupancySensing", 1, {rep.data(), rep.size()});
    assert(r.any_matched);
    const Value* o = r.merged.find("occupancy");
    assert(o && o->type == ValueType::Bool && o->b == true);

    // bit0=0 -> clear.
    const std::array<std::uint8_t, 7> rep0 = {0x18, 0x11, 0x0A, 0x00, 0x00, 0x18, 0x00};
    auto r0 = dispatch_report(def, 0x0406, "msOccupancySensing", 1, {rep0.data(), rep0.size()});
    assert(r0.any_matched);
    const Value* o0 = r0.merged.find("occupancy");
    assert(o0 && o0->type == ValueType::Bool && o0->b == false);
}

// ── ERCU_WS_Zm — wall remote: action suffixed per endpoint ───────────
void test_ercu_ws_endpoint_suffix() {
    const auto& def = devices::awox::kDef_ERCU_WS_Zm;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));        // not a controllable switch
    assert(def.to_zigbee_count == 0);          // no on/off TZ
    assert(def.endpoint_map_count == 2);
    assert(def.endpoint_action_suffix);
    assert(def_binds(def, 0x0300));            // lightingColorCtrl bound

    // genOnOff On (0x01) on endpoint 1 -> action_1 = "on".
    const std::array<std::uint8_t, 3> on = {0x01, 0x42, 0x01};
    auto r1 = dispatch_cmd(def, 0x0006, "genOnOff", 1, {on.data(), on.size()});
    assert(r1.any_matched);
    const char* a1 = str_of(r1, "action_1");
    assert(a1 && std::strcmp(a1, "on") == 0);
    assert(r1.merged.find("action") == nullptr);   // bare key suffixed away

    // genOnOff Off (0x00) on endpoint 3 -> action_3 = "off".
    const std::array<std::uint8_t, 3> off = {0x01, 0x42, 0x00};
    auto r3 = dispatch_cmd(def, 0x0006, "genOnOff", 3, {off.data(), off.size()});
    assert(r3.any_matched);
    const char* a3 = str_of(r3, "action_3");
    assert(a3 && std::strcmp(a3, "off") == 0);
}

// ── ERCU_WS_Zm — level + colour command families surface as action ───
void test_ercu_ws_level_color() {
    const auto& def = devices::awox::kDef_ERCU_WS_Zm;

    // genLevelCtrl Move (cmd 0x01) mode=0 (up), rate=50 on ep1
    //   -> action_1 = "brightness_move_up".
    const std::array<std::uint8_t, 5> mv = {0x01, 0x42, 0x01, 0x00, 0x32};
    auto rm = dispatch_cmd(def, 0x0008, "genLevelCtrl", 1, {mv.data(), mv.size()});
    assert(rm.any_matched);
    const char* am = str_of(rm, "action_1");
    assert(am && std::strcmp(am, "brightness_move_up") == 0);

    // lightingColorCtrl MoveToColorTemp (cmd 0x0A): colortemp u16=370 LE,
    // transition u16=0 -> action_1 = "color_temperature_move".
    const std::array<std::uint8_t, 7> ct = {0x01, 0x42, 0x0A, 0x72, 0x01, 0x00, 0x00};
    auto rc = dispatch_cmd(def, 0x0300, "lightingColorCtrl", 1, {ct.data(), ct.size()});
    assert(rc.any_matched);
    const char* ac = str_of(rc, "action_1");
    assert(ac && std::strcmp(ac, "color_temperature_move") == 0);
}

// ── 33952 — remote controller: command stream → action ───────────────
void test_33952_actions() {
    const auto& def = devices::awox::kDef_D33952;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));        // not a controllable switch
    assert(def.to_zigbee_count == 0);          // no on/off TZ
    assert(def_binds(def, 0x0005));            // genScenes bound (recall)

    // genOnOff On (0x01) -> "on".
    const std::array<std::uint8_t, 3> on = {0x01, 0x42, 0x01};
    auto ron = dispatch_cmd(def, 0x0006, "genOnOff", 1, {on.data(), on.size()});
    assert(ron.any_matched);
    const char* aon = str_of(ron, "action");
    assert(aon && std::strcmp(aon, "on") == 0);
    assert(ron.merged.find("state") == nullptr);   // raw state never published

    // genScenes Recall (cmd 0x05): group=0x0000 LE, scene=2 -> "recall_2".
    const std::array<std::uint8_t, 6> rc = {0x01, 0x42, 0x05, 0x00, 0x00, 0x02};
    auto rrc = dispatch_cmd(def, 0x0005, "genScenes", 1, {rc.data(), rc.size()});
    assert(rrc.any_matched);
    const char* arc = str_of(rrc, "action");
    assert(arc && std::strcmp(arc, "recall_2") == 0);
}

}  // namespace

int main() {
    test_epir_occupancy();
    test_ercu_ws_endpoint_suffix();
    test_ercu_ws_level_color();
    test_33952_actions();
    return 0;
}
