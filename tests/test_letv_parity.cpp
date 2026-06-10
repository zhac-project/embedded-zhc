// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the LeTV (Leeco) 8-key scene remote (qlwz.letv8key.10).
// Pins the real gaps the auto-generated port carried against z2m:
//
//   * The port mis-ported the remote as a controllable on/off SWITCH:
//     a phantom genOnOff in/out (kFzOnOff + kTzOnOff + a settable `state`
//     Binary expose + 0x0006 binding + to_zigbee). The device has NO
//     on/off — z2m: fromZigbee=[fz.qlwz_letv8key_switch], toZigbee=[].
//   * The action channel was entirely dropped: z2m decodes
//     genMultistateInput.presentValue (attr 0x0055), keyed by the source
//     endpoint, into "<gesture>_<button>" (e.g. "single_up"). Restored
//     with kFzLetv8KeyAction. Button lives INSIDE the action string, so
//     there is no endpoint_map / action suffix.
//
// z2m-source: letv.ts (#LeTV.8KEY) + fromZigbee.ts (qlwz_letv8key_switch).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::letv {
extern const PreparedDefinition kDef_LeTV_8KEY;
}  // namespace zhc::devices::letv

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

// Report Attributes (cmd 0x0A) for genMultistateInput presentValue
// (attr 0x0055), data type uint16 (0x21) → 2 value bytes LE.
struct MsFrame { std::array<std::uint8_t, 8> bytes; };

MsFrame ms_present(std::uint16_t value, std::uint8_t tsn = 0x42) {
    MsFrame f{};
    f.bytes = { 0x18, tsn, 0x0A,            // fc, tsn, cmd=Report Attributes
                0x55, 0x00,                 // attr 0x0055
                0x21,                       // type uint16
                static_cast<std::uint8_t>(value & 0xFF),
                static_cast<std::uint8_t>(value >> 8) };
    return f;
}

DispatchResult dispatch_ms(const PreparedDefinition& def, std::uint8_t src_ep,
                           const MsFrame& f) {
    auto raw = make_frame(0x0012, src_ep,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genMultistateInput";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool action_is(const Value* v, const char* expect) {
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

}  // namespace

// ── shape: action enum only; NO phantom on/off, no to_zigbee/binding ──
static void test_shape() {
    const auto& def = devices::letv::kDef_LeTV_8KEY;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));   // phantom on/off expose gone

    // Uncontrollable scene remote — no to_zigbee.
    assert(def.to_zigbee_count == 0);

    // No genOnOff (0x0006) binding survived (z2m binds nothing here).
    bool binds_onoff = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == 0x0006) binds_onoff = true;
    assert(!binds_onoff);
    assert(def.bindings_count == 0);

    // Button is encoded in the action string → NO endpoint_map.
    assert(def.endpoint_map_count == 0);
    assert(!def.endpoint_action_suffix);

    // Fingerprint preserved.
    assert(def.zigbee_models_count == 1);
    assert(std::strcmp(def.zigbee_models[0], "qlwz.letv8key.10") == 0);
}

// ── endpoint → button + presentValue → gesture composition ───────────
static void test_actions() {
    const auto& def = devices::letv::kDef_LeTV_8KEY;

    // z2m buttonLookup {4:up,2:down,5:left,3:right,8:center,1:back,7:play,
    // 6:voice}; actionLookup {0:hold,1:single,2:double,3:tripple}.
    struct Case { std::uint8_t ep; std::uint16_t pv; const char* expect; };
    constexpr Case kCases[] = {
        {4, 1, "single_up"},      // EP4 single
        {4, 0, "hold_up"},        // gesture 0 = hold
        {2, 2, "double_down"},    // EP2 double
        {5, 3, "tripple_left"},   // EP5 triple (z2m spelling "tripple")
        {3, 1, "single_right"},   // EP3
        {8, 0, "hold_center"},    // EP8
        {1, 2, "double_back"},    // EP1
        {7, 1, "single_play"},    // EP7
        {6, 3, "tripple_voice"},  // EP6
    };
    for (const auto& c : kCases) {
        auto r = dispatch_ms(def, c.ep, ms_present(c.pv));
        assert(r.any_matched);
        assert(action_is(r.merged.find("action"), c.expect));
    }
}

// ── out-of-range endpoint / gesture are dropped (no phantom key) ─────
static void test_drops() {
    const auto& def = devices::letv::kDef_LeTV_8KEY;

    // Endpoint 9 maps to no button in z2m's buttonLookup → no emit.
    auto r1 = dispatch_ms(def, 9, ms_present(1));
    assert(r1.merged.find("action") == nullptr);

    // presentValue 4 is outside {0..3} → no numeric fallthrough action.
    auto r2 = dispatch_ms(def, 4, ms_present(4));
    assert(r2.merged.find("action") == nullptr);
}

int main() {
    test_shape();
    test_actions();
    test_drops();
    return 0;
}
