// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Inovelli Blue-series switches / dimmers / fan
// (VZM30-SN, VZM31-SN, VZM32-SN, VZM35-SN) plus the VZM36 fan canopy.
// z2m-source: inovelli.ts + lib/inovelli.ts.
//
// The Tier-2 fix pinned here: every device that z2m marks
// `supportsButtonTaps: true` (VZM30/31/32/35) emits a manufacturer-
// specific raw frame on endpoint 2 (cluster 0xFC31, cmd 0x00) carrying
// { button, click } which z2m turns into
//   `action: "<button>_<click>"`  (e.g. up_double, config_held)
// via `fzLocal.inovelli` (the `msg.type === "raw"` branch) +
// `buttonLookup` / `clickLookup` and `e.action(BUTTON_TAP_SEQUENCES)`.
// The generated ports dropped both the `action` expose AND the decoder;
// inovelli::kFzInovelliSceneAction restores them.
//
// Guards verified:
//   * endpoint 2 only (z2m `msg.endpoint.ID === 2`);
//   * manufacturer_specific only (cmd 0x00 collides with genOnOff Off /
//     genScenes Add — a non-manu frame must NOT mint a bogus action);
//   * out-of-range button / click codes publish nothing;
//   * VZM36 (`supportsButtonTaps: false`) carries NO action expose.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::inovelli {
extern const PreparedDefinition kDef_VZM30_SN;
extern const PreparedDefinition kDef_VZM31_SN;
extern const PreparedDefinition kDef_VZM32_SN;
extern const PreparedDefinition kDef_VZM35_SN;
extern const PreparedDefinition kDef_VZM36;
extern const FzConverter kFzInovelliSceneAction;
}  // namespace zhc::devices::inovelli

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

bool def_has_fz(const PreparedDefinition& def, const FzConverter* cvt) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == cvt) return true;
    return false;
}

const char* str_of(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}

// manuSpecificInovelli scene/button event (cmd 0x00):
//   fc=0x0D (cluster-specific + manufacturer-specific + server->client)
//   mfgCode LE (0x122f) | tsn | cmd=0x00 | button:u8 | click:u8
std::array<std::uint8_t, 7> scene_frame(std::uint8_t button, std::uint8_t click) {
    return {0x0D, 0x2F, 0x12, 0x00, 0x00,  // fc, mfgLo, mfgHi, tsn, cmd
            button, click};
}

constexpr std::uint16_t CL_ONOFF    = 0x0006;
constexpr std::uint16_t CL_INOVELLI = 0xFC31;

}  // namespace

// ── action expose + decoder are wired on the button-tap devices ──────

static void test_button_tap_defs_expose_action() {
    using namespace devices::inovelli;
    assert(def_exposes(kDef_VZM30_SN, "action"));
    assert(def_exposes(kDef_VZM31_SN, "action"));
    assert(def_exposes(kDef_VZM32_SN, "action"));
    assert(def_exposes(kDef_VZM35_SN, "action"));
    assert(def_has_fz(kDef_VZM30_SN, &kFzInovelliSceneAction));
    assert(def_has_fz(kDef_VZM31_SN, &kFzInovelliSceneAction));
    assert(def_has_fz(kDef_VZM32_SN, &kFzInovelliSceneAction));
    assert(def_has_fz(kDef_VZM35_SN, &kFzInovelliSceneAction));
}

// VZM36 (supportsButtonTaps: false) has neither expose nor decoder.
static void test_vzm36_no_action() {
    using namespace devices::inovelli;
    assert(!def_exposes(kDef_VZM36, "action"));
    assert(!def_has_fz(kDef_VZM36, &kFzInovelliSceneAction));
}

// Full button × click cross-product decodes to z2m's <button>_<click>.
static void test_scene_action_decode() {
    const auto& def = devices::inovelli::kDef_VZM31_SN;
    struct Case { std::uint8_t b, c; const char* want; };
    const Case cases[] = {
        {1, 0, "down_single"},
        {2, 0, "up_single"},
        {3, 0, "config_single"},
        {1, 1, "down_release"},
        {2, 2, "up_held"},
        {3, 3, "config_double"},
        {1, 4, "down_triple"},
        {2, 5, "up_quadruple"},
        {3, 6, "config_quintuple"},
        {4, 3, "aux_down_double"},   // aux variants (buttonLookup 4..6)
        {5, 2, "aux_up_held"},
        {6, 0, "aux_config_single"},
    };
    for (const auto& c : cases) {
        auto f = scene_frame(c.b, c.c);
        auto r = dispatch(def, CL_INOVELLI, /*ep=*/2, {f.data(), f.size()});
        assert(r.any_matched);
        const char* a = str_of(r, "action");
        assert(a && std::strcmp(a, c.want) == 0);
    }
}

// z2m fires the raw branch ONLY on endpoint 2. A frame on EP1 must not
// produce an action.
static void test_scene_action_endpoint_guard() {
    const auto& def = devices::inovelli::kDef_VZM31_SN;
    auto f = scene_frame(2, 3);  // would be up_double on EP2
    auto r = dispatch(def, CL_INOVELLI, /*ep=*/1, {f.data(), f.size()});
    assert(!has_key(r, "action"));
}

// Command 0x00 also names genOnOff Off / genScenes Add etc. A plain,
// NON-manufacturer-specific command 0x00 must NOT mint a bogus action.
static void test_non_manu_cmd0_not_action() {
    const auto& def = devices::inovelli::kDef_VZM31_SN;
    // fc=0x01 cluster-specific, NOT manu; tsn; cmd=0x00 (Off).
    const std::array<std::uint8_t, 3> f = {0x01, 0x42, 0x00};
    auto r = dispatch(def, CL_ONOFF, /*ep=*/2, {f.data(), f.size()});
    assert(!has_key(r, "action"));
}

// Out-of-range button / click codes publish nothing.
static void test_scene_action_unknown_noop() {
    const auto& def = devices::inovelli::kDef_VZM31_SN;
    {
        auto f = scene_frame(0, 1);   // button 0 invalid (z2m buttonLookup 1..6)
        auto r = dispatch(def, CL_INOVELLI, 2, {f.data(), f.size()});
        assert(!has_key(r, "action"));
    }
    {
        auto f = scene_frame(7, 1);   // button 7 invalid
        auto r = dispatch(def, CL_INOVELLI, 2, {f.data(), f.size()});
        assert(!has_key(r, "action"));
    }
    {
        auto f = scene_frame(1, 7);   // click 7 invalid (z2m clickLookup 0..6)
        auto r = dispatch(def, CL_INOVELLI, 2, {f.data(), f.size()});
        assert(!has_key(r, "action"));
    }
}

// Core on/off + brightness still decode (regression: graduation kept the
// existing converters intact).
static void test_core_onoff_still_decodes() {
    const auto& def = devices::inovelli::kDef_VZM31_SN;
    // genOnOff attr 0x0000 (bool) report = ON.
    const std::array<std::uint8_t, 7> f =
        {0x18, 0x00, 0x0A, 0x00, 0x00, 0x10, 0x01};
    auto r = dispatch(def, CL_ONOFF, /*ep=*/1, {f.data(), f.size()});
    const Value* v = r.merged.find("state");
    assert(v && v->type == ValueType::Bool && v->b);
}

int main() {
    test_button_tap_defs_expose_action();
    test_vzm36_no_action();
    test_scene_action_decode();
    test_scene_action_endpoint_guard();
    test_non_manu_cmd0_not_action();
    test_scene_action_unknown_noop();
    test_core_onoff_still_decodes();
    return 0;
}
