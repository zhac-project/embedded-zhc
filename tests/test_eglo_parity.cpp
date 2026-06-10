// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the EGLO connect family. z2m-source:
// zigbee-herdsman-converters/src/devices/eglo.ts. EGLO is an AwoX rebadge.
//
// Two gap classes were fixed:
//
//  * Light completeness — every EGLO bulb is z2m m.light({colorTemp, ...})
//    but the generator lowered all five onto on/off + brightness only, so
//    the lightingColorCtrl (0x0300) color_temp / colour reports were dead.
//    Re-wired to eglo/_shared CT (12242, 98847) and full-RGBW (300686,
//    900091, 900024/12253) bundles. These tests pin color_temp + colour
//    decode and the 0x0300 binding.
//
//  * Remote-as-switch — 99099 (3-groups remote) and 99106 (TLSR82xx) are
//    z2m command transmitters exposing `action` via the genOnOff /
//    genLevelCtrl / lightingColorCtrl command converters. The generated
//    defs wired kFzOnOff + an on/off TZ (both dead) and exposed a
//    meaningless `state`. Re-wired to the generic command-action decoders.
//    These tests pin the action stream and the absence of a dead `state` /
//    on/off TZ. The AwoX-specific raw colour/refresh frames (awox_colors /
//    awox_refresh / awox_refreshColored) have no generic equivalent and are
//    intentionally deferred.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::eglo {
extern const PreparedDefinition kDef_D12242;          // ST64 CT filament bulb
extern const PreparedDefinition kDef_D98847;          // FUEVA-Z CT ceiling light
extern const PreparedDefinition kDef_D300686;         // MASSIGNANO-Z RGBW
extern const PreparedDefinition kDef_D900091;         // ROVITO-Z RGBW
extern const PreparedDefinition kDef_D900024_12253;   // RGBW light
extern const PreparedDefinition kDef_D99099;          // 3-groups remote
extern const PreparedDefinition kDef_D99106;          // TLSR82xx remote
}  // namespace zhc::devices::eglo

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

// Report-Attributes frame (server->client, fc=0x18). Decoder leaves cluster
// numeric; the named-cluster converters match on the string, so the caller
// stamps the cluster name post-decode (mirrors test_awox_parity).
//   bytes = fc(1)=0x18 | tsn(1) | cmd(1)=0x0A | <attr recs...>
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

// Cluster-specific command frame (client->server, fc=0x01). Decoder leaves
// cluster numeric; the named-cluster command converters match on the string
// (and cmd-id 0x00 collides between genOnOff Off and genLevelCtrl
// MoveToLevel), so the caller stamps the cluster name post-decode.
//   bytes = fc(1)=0x01 | tsn(1) | cmd(1) | body...
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

// ── CT bulbs: color_temp report decodes ──────────────────────────────
void test_ct_light_color_temp() {
    const auto& def = devices::eglo::kDef_D12242;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));      // restored channel
    assert(!def_exposes(def, "color_x"));        // CT-only, no colour
    assert(def_binds(def, 0x0300));              // lightingColorCtrl bound
    assert(def.to_zigbee_count == 3);            // on/off + brightness + CT set

    // Report Attributes on lightingColorCtrl: fc=0x18 | tsn | cmd=0x0A |
    // attr 0x0007 LE | type 0x21 (u16) | value 0x0172 (=370 mireds) LE.
    const std::array<std::uint8_t, 8> rep =
        {0x18, 0x10, 0x0A, 0x07, 0x00, 0x21, 0x72, 0x01};
    auto r = dispatch_report(def, 0x0300, "lightingColorCtrl", 1, {rep.data(), rep.size()});
    assert(r.any_matched);
    const Value* ct = r.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 370);

    // 98847 (FUEVA-Z) shares the CT bundle.
    assert(def_exposes(devices::eglo::kDef_D98847, "color_temp"));
    assert(def_binds(devices::eglo::kDef_D98847, 0x0300));
}

// ── RGBW bulbs: colour (hue) + color_temp report decodes ──────────────
void test_rgbw_light_color() {
    const auto& def = devices::eglo::kDef_D300686;
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));
    assert(def_exposes(def, "color_x"));
    assert(def_binds(def, 0x0300));
    assert(def.to_zigbee_count == 4);            // on/off + brightness + CT + colour set

    // Report Attributes on lightingColorCtrl: attr 0x0000 currentHue
    // (type 0x20 u8) value 0x7F (=127).
    const std::array<std::uint8_t, 7> rep =
        {0x18, 0x10, 0x0A, 0x00, 0x00, 0x20, 0x7F};
    auto r = dispatch_report(def, 0x0300, "lightingColorCtrl", 1, {rep.data(), rep.size()});
    assert(r.any_matched);
    const Value* h = r.merged.find("hue");
    assert(h && h->type == ValueType::Uint && h->u == 127);

    // 900091 + 900024/12253 share the full-RGBW bundle.
    assert(def_exposes(devices::eglo::kDef_D900091, "color_temp"));
    assert(def_exposes(devices::eglo::kDef_D900091, "hue"));
    assert(def_exposes(devices::eglo::kDef_D900024_12253, "color_temp"));
    assert(def_exposes(devices::eglo::kDef_D900024_12253, "hue"));
}

// ── 99099 — 3-groups remote: command stream → action ──────────────────
void test_99099_actions() {
    const auto& def = devices::eglo::kDef_D99099;
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "action_group"));
    assert(!def_exposes(def, "state"));          // not a controllable switch
    assert(def.to_zigbee_count == 0);            // no on/off TZ
    assert(def_binds(def, 0x0005));              // genScenes bound (recall)
    assert(def_binds(def, 0x0300));              // lightingColorCtrl bound (CT step)

    // genOnOff On (0x01) -> "on".
    const std::array<std::uint8_t, 3> on = {0x01, 0x42, 0x01};
    auto ron = dispatch_cmd(def, 0x0006, "genOnOff", 1, {on.data(), on.size()});
    assert(ron.any_matched);
    const char* aon = str_of(ron, "action");
    assert(aon && std::strcmp(aon, "on") == 0);
    assert(ron.merged.find("state") == nullptr); // raw state never published

    // genLevelCtrl MoveToLevel (cmd 0x00): level=128, transition u16=0
    //   -> "brightness_move_to_level".
    const std::array<std::uint8_t, 6> ml = {0x01, 0x42, 0x00, 0x80, 0x00, 0x00};
    auto rml = dispatch_cmd(def, 0x0008, "genLevelCtrl", 1, {ml.data(), ml.size()});
    assert(rml.any_matched);
    const char* aml = str_of(rml, "action");
    assert(aml && std::strcmp(aml, "brightness_move_to_level") == 0);

    // genScenes Recall (cmd 0x05): group=0x0000 LE, scene=1 -> "recall_1".
    const std::array<std::uint8_t, 6> rc = {0x01, 0x42, 0x05, 0x00, 0x00, 0x01};
    auto rrc = dispatch_cmd(def, 0x0005, "genScenes", 1, {rc.data(), rc.size()});
    assert(rrc.any_matched);
    const char* arc = str_of(rrc, "action");
    assert(arc && std::strcmp(arc, "recall_1") == 0);
}

// ── 99106 — TLSR82xx remote: command stream → action ──────────────────
void test_99106_actions() {
    const auto& def = devices::eglo::kDef_D99106;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));          // not a controllable switch
    assert(def.to_zigbee_count == 0);            // no on/off TZ
    assert(def_binds(def, 0x0300));              // lightingColorCtrl bound

    // genOnOff On (0x01) -> "on".
    const std::array<std::uint8_t, 3> on = {0x01, 0x42, 0x01};
    auto ron = dispatch_cmd(def, 0x0006, "genOnOff", 1, {on.data(), on.size()});
    assert(ron.any_matched);
    const char* aon = str_of(ron, "action");
    assert(aon && std::strcmp(aon, "on") == 0);

    // lightingColorCtrl MoveToColorTemp (cmd 0x0A): colortemp u16=370 LE,
    // transition u16=0 -> "color_temperature_move".
    const std::array<std::uint8_t, 7> ct = {0x01, 0x42, 0x0A, 0x72, 0x01, 0x00, 0x00};
    auto rc = dispatch_cmd(def, 0x0300, "lightingColorCtrl", 1, {ct.data(), ct.size()});
    assert(rc.any_matched);
    const char* ac = str_of(rc, "action");
    assert(ac && std::strcmp(ac, "color_temperature_move") == 0);
}

}  // namespace

int main() {
    test_ct_light_color_temp();
    test_rgbw_light_color();
    test_99099_actions();
    test_99106_actions();
    return 0;
}
