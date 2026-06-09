// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Siglis zigfred wall controllers (ZFP-1A-CH plus,
// ZFU-1D-CH uno). z2m-source: siglis.ts.
//
// These pin the two Tier-2 parity fixes on top of the existing
// multi-endpoint hand-rewrite:
//
//   (1) EP5 (`l1`, zigfredEndpoint) is present in `endpoint_map`, so the
//       front-surface RGB LED's state/brightness/color reports get the
//       `_l1` suffix and match the declared state_l1/brightness_l1/
//       color_l1 exposes. Before the fix EP5 had no map entry → reports
//       stayed bare (`state`) and never matched the exposes.
//
//   (2) Button presses on the manufacturer-specific cluster
//       (manuSpecificSiglisZigfred 0xFC42, cmd 0x02
//       siglisZigfredButtonEvent) decode through
//       siglis::kFzZigfredButtonEvent into `action: "button_<n>_<event>"`
//       — z2m's `zifgredFromZigbeeButtonEvent`. Previously undecoded.
//
// Plus the standing composite-completeness assertions: every dimmer /
// cover / relay endpoint suffixes correctly, and the genOnOff-Toggle
// command (non-manu, also cmd 0x02) is NOT hijacked by the button
// decoder.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::siglis {
extern const PreparedDefinition kDef_ZFP_1A_CH;  // zigfred plus
extern const PreparedDefinition kDef_ZFU_1D_CH;  // zigfred uno
}  // namespace zhc::devices::siglis

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

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster,
                        std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
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

std::uint8_t ep_of(const PreparedDefinition& def, const char* label) {
    for (std::uint8_t i = 0; i < def.endpoint_map_count; ++i)
        if (std::strcmp(def.endpoint_map[i].label, label) == 0) return def.endpoint_map[i].endpoint;
    return 0xFF;  // not mapped
}

const char* str_of(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    if (!v) return nullptr;
    if (v->type == ValueType::StringRef) return v->str;
    return nullptr;
}

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}

// genOnOff On/Off attribute report: attr 0x0000 (bool) = value.
std::array<std::uint8_t, 7> onoff_report(bool on) {
    return {0x18, 0x00, 0x0A,        // fc(server->client report), tsn, cmd=Report
            0x00, 0x00,              // attr 0x0000
            0x10,                    // type bool
            static_cast<std::uint8_t>(on ? 0x01 : 0x00)};
}

// closuresWindowCovering currentPositionLiftPercentage: attr 0x0008 (u8).
std::array<std::uint8_t, 7> lift_report(std::uint8_t pct) {
    return {0x18, 0x00, 0x0A,        // report
            0x08, 0x00,              // attr 0x0008
            0x20,                    // type u8
            pct};
}

// manuSpecificSiglisZigfred siglisZigfredButtonEvent (cmd 0x02):
//   fc=0x05 (cluster-specific + manufacturer-specific, client->server)
//   mfgCode LE | tsn | cmd=0x02 | button:u8 | type:u8 | duration:u16 LE
std::array<std::uint8_t, 9> button_frame(std::uint8_t button, std::uint8_t type) {
    return {0x05, 0x09, 0x14, 0x00, 0x02,  // fc, mfgLo, mfgHi, tsn, cmd
            button, type, 0x00, 0x00};     // button, type, duration LE
}

constexpr std::uint16_t CL_ONOFF  = 0x0006;
constexpr std::uint16_t CL_LEVEL  = 0x0008;
constexpr std::uint16_t CL_COVER  = 0x0102;
constexpr std::uint16_t CL_SIGLIS = 0xFC42;

}  // namespace

// ── ZFP-1A-CH (zigfred plus) ─────────────────────────────────────────

// l1=EP5 must be in the endpoint map (the fix). All 7 labels present.
static void test_zfp_endpoint_map() {
    const auto& def = devices::siglis::kDef_ZFP_1A_CH;
    assert(def.endpoint_map_count == 7);
    assert(ep_of(def, "l1") == 5);   // <-- the fix: was missing
    assert(ep_of(def, "l2") == 7);
    assert(ep_of(def, "l3") == 8);
    assert(ep_of(def, "l4") == 9);
    assert(ep_of(def, "l5") == 10);
    assert(ep_of(def, "l6") == 11);
    assert(ep_of(def, "l7") == 12);
}

// Every endpoint's exposes are declared (composite completeness).
static void test_zfp_exposes_complete() {
    const auto& def = devices::siglis::kDef_ZFP_1A_CH;
    assert(def_exposes(def, "action"));
    // l1 RGB LED.
    assert(def_exposes(def, "state_l1"));
    assert(def_exposes(def, "brightness_l1"));
    assert(def_exposes(def, "color_l1"));
    // 4 dimmers l2..l5.
    assert(def_exposes(def, "state_l2") && def_exposes(def, "brightness_l2"));
    assert(def_exposes(def, "state_l3") && def_exposes(def, "brightness_l3"));
    assert(def_exposes(def, "state_l4") && def_exposes(def, "brightness_l4"));
    assert(def_exposes(def, "state_l5") && def_exposes(def, "brightness_l5"));
    // 2 covers l6,l7 with position.
    assert(def_exposes(def, "state_l6") && def_exposes(def, "position_l6"));
    assert(def_exposes(def, "state_l7") && def_exposes(def, "position_l7"));
    // Every bound cluster from the z2m configure block.
    assert(def_binds(def, 5, CL_SIGLIS));
    assert(def_binds(def, 5, 0x0300));    // genColorCtrl on EP5
    assert(def_binds(def, 11, CL_COVER) && def_binds(def, 12, CL_COVER));
}

// EP5 on/off now suffixes to state_l1 (the fix). Before, no map entry for
// EP5 left it bare `state`.
static void test_zfp_l1_onoff_suffixed() {
    const auto& def = devices::siglis::kDef_ZFP_1A_CH;
    auto f = onoff_report(true);
    auto r = dispatch(def, CL_ONOFF, /*ep=*/5, {f.data(), f.size()});
    assert(r.any_matched);
    const Value* v = r.merged.find("state_l1");
    assert(v && v->type == ValueType::Bool && v->b);
    assert(!has_key(r, "state"));         // bare key was suffixed away
}

// Dimmer 1 on EP7 → state_l2.
static void test_zfp_dimmer_suffixed() {
    const auto& def = devices::siglis::kDef_ZFP_1A_CH;
    auto f = onoff_report(false);
    auto r = dispatch(def, CL_ONOFF, /*ep=*/7, {f.data(), f.size()});
    assert(r.any_matched);
    const Value* v = r.merged.find("state_l2");
    assert(v && v->type == ValueType::Bool && !v->b);
}

// Cover 1 on EP11 → position_l6.
static void test_zfp_cover_position_suffixed() {
    const auto& def = devices::siglis::kDef_ZFP_1A_CH;
    auto f = lift_report(42);
    auto r = dispatch(def, CL_COVER, /*ep=*/11, {f.data(), f.size()});
    assert(r.any_matched);
    assert(has_key(r, "position_l6"));
    assert(!has_key(r, "position"));      // suffixed
    // Cover 2 on EP12 → position_l7.
    auto r2 = dispatch(def, CL_COVER, /*ep=*/12, {f.data(), f.size()});
    assert(has_key(r2, "position_l7"));
}

// Button events on the manu cluster → action "button_<n>_<event>".
static void test_zfp_button_actions() {
    const auto& def = devices::siglis::kDef_ZFP_1A_CH;
    struct Case { std::uint8_t b, t; const char* want; };
    const Case cases[] = {
        {0, 1, "button_1_single"},
        {1, 2, "button_2_double"},
        {2, 3, "button_3_hold"},
        {3, 0, "button_4_release"},
    };
    for (const auto& c : cases) {
        auto f = button_frame(c.b, c.t);
        auto r = dispatch(def, CL_SIGLIS, /*ep=*/5, {f.data(), f.size()});
        assert(r.any_matched);
        const char* a = str_of(r, "action");
        assert(a && std::strcmp(a, c.want) == 0);
        // `action` is global (value encodes the button) — NOT suffixed.
        assert(!has_key(r, "action_l1"));
    }
}

// Unknown button / type codes publish nothing.
static void test_zfp_button_unknown_noop() {
    const auto& def = devices::siglis::kDef_ZFP_1A_CH;
    auto f = button_frame(7, 1);  // button 7 out of range
    auto r = dispatch(def, CL_SIGLIS, /*ep=*/5, {f.data(), f.size()});
    assert(!has_key(r, "action"));
}

// genOnOff Toggle (cmd 0x02, NON manufacturer-specific) must NOT be
// hijacked by the button decoder — the manufacturer_specific guard.
static void test_zfp_genonoff_toggle_not_button() {
    const auto& def = devices::siglis::kDef_ZFP_1A_CH;
    // fc=0x01 cluster-specific, NOT manu; tsn; cmd=0x02 Toggle.
    const std::array<std::uint8_t, 3> f = {0x01, 0x42, 0x02};
    auto r = dispatch(def, CL_ONOFF, /*ep=*/5, {f.data(), f.size()});
    // Whatever else happens, the siglis decoder must not have emitted a
    // bogus button action from a plain Toggle.
    assert(!has_key(r, "action"));
}

// ── ZFU-1D-CH (zigfred uno) ──────────────────────────────────────────

static void test_zfu_endpoint_map_and_exposes() {
    const auto& def = devices::siglis::kDef_ZFU_1D_CH;
    assert(def.endpoint_map_count == 3);
    assert(ep_of(def, "l1") == 5);   // <-- the fix
    assert(ep_of(def, "l2") == 6);   // relay
    assert(ep_of(def, "l3") == 7);   // dimmer
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "state_l1") && def_exposes(def, "brightness_l1") &&
           def_exposes(def, "color_l1"));
    assert(def_exposes(def, "state_l2"));                       // relay
    assert(def_exposes(def, "state_l3") && def_exposes(def, "brightness_l3"));  // dimmer
    assert(def_binds(def, 5, CL_SIGLIS));
}

// EP5 on/off → state_l1; relay EP6 → state_l2; dimmer level EP7 → brightness_l3.
static void test_zfu_endpoints_decode() {
    const auto& def = devices::siglis::kDef_ZFU_1D_CH;
    {
        auto f = onoff_report(true);
        auto r = dispatch(def, CL_ONOFF, 5, {f.data(), f.size()});
        const Value* v = r.merged.find("state_l1");
        assert(v && v->type == ValueType::Bool && v->b);
    }
    {
        auto f = onoff_report(true);
        auto r = dispatch(def, CL_ONOFF, 6, {f.data(), f.size()});
        assert(has_key(r, "state_l2"));
    }
    {
        // genLevelCtrl currentLevel attr 0x0000 (u8) = 128 → brightness_l3.
        const std::array<std::uint8_t, 7> f =
            {0x18, 0x00, 0x0A, 0x00, 0x00, 0x20, 0x80};
        auto r = dispatch(def, CL_LEVEL, 7, {f.data(), f.size()});
        assert(has_key(r, "brightness_l3"));
    }
}

static void test_zfu_button_action() {
    const auto& def = devices::siglis::kDef_ZFU_1D_CH;
    auto f = button_frame(0, 2);  // button_1_double
    auto r = dispatch(def, CL_SIGLIS, 5, {f.data(), f.size()});
    assert(r.any_matched);
    const char* a = str_of(r, "action");
    assert(a && std::strcmp(a, "button_1_double") == 0);
}

int main() {
    test_zfp_endpoint_map();
    test_zfp_exposes_complete();
    test_zfp_l1_onoff_suffixed();
    test_zfp_dimmer_suffixed();
    test_zfp_cover_position_suffixed();
    test_zfp_button_actions();
    test_zfp_button_unknown_noop();
    test_zfp_genonoff_toggle_not_button();
    test_zfu_endpoint_map_and_exposes();
    test_zfu_endpoints_decode();
    test_zfu_button_action();
    return 0;
}
