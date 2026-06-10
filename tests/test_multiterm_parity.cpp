// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Multiterm ZC0101 ZeeFan fan coil unit controller.
//
// The auto-port modelled this as a bare single-binary `fan_mode` device and
// dropped the entire multi-endpoint binary-output half. z2m exposes a fan
// control (hvacFanCtrl 0x0202) plus three genBinaryOutput (0x0010) channels
// on endpoints 8/9/10 (silent_mode / heating_cooling / electric_valve) via
// `fzLocal.binary_output` under m.deviceEndpoints({8,9,10}).
//
// Fixed: wired the generic kFzBinaryOutput (presentValue → boolean `state`)
// and added an endpoint_map for ep9/ep10 so the three channels stop
// colliding on a bare `state` key. ep8 is intentionally unmapped so the fan
// control's hvacFanCtrl key (fan_mode) stays unsuffixed, matching z2m.
//
// These tests pin, on real genBinaryOutput / hvacFanCtrl wire shapes, that
//   * the genBinaryOutput presentValue decodes to a boolean state,
//   * the endpoint_map suffixes ep9 -> state_9, ep10 -> state_10 while ep8
//     stays bare `state`,
//   * the fan_mode key from hvacFanCtrl on ep8 is NOT suffixed, and
//   * the def carries the z2m exposes (fan_state + the three channel enums)
//     and the bogus bare `fan_mode` expose is gone.
//
// z2m-source: multiterm.ts #ZC0101.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::multiterm {
extern const PreparedDefinition kDef_ZC0101;
}  // namespace zhc::devices::multiterm

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
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

// genBinaryOutput Report Attributes: presentValue (attr 0x0055, Boolean 0x10).
//   fc(1)=0x18 (profile-wide, S->C) | tsn(1) | cmd(1)=0x0A (Report Attributes)
//   attr_le(2)=0x55 0x00 | type(1)=0x10 (Boolean) | value(1)
struct BinOutFrame { std::array<std::uint8_t, 7> bytes; };

BinOutFrame bin_out(std::uint8_t value, std::uint8_t tsn = 0x42) {
    BinOutFrame f{};
    f.bytes = {0x18, tsn, 0x0A, 0x55, 0x00, 0x10, value};
    return f;
}

DispatchResult run_bin_out(const PreparedDefinition& def, std::uint8_t src_ep,
                           std::uint8_t value) {
    const BinOutFrame bf = bin_out(value);
    auto raw = make_frame(0x0010 /* genBinaryOutput */, src_ep,
                          std::span<const std::uint8_t>(bf.bytes.data(),
                                                        bf.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genBinaryOutput";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// hvacFanCtrl Report Attributes: fanMode (attr 0x0000, enum8 0x30).
struct FanFrame { std::array<std::uint8_t, 7> bytes; };

FanFrame fan_mode(std::uint8_t mode, std::uint8_t tsn = 0x42) {
    FanFrame f{};
    f.bytes = {0x18, tsn, 0x0A, 0x00, 0x00, 0x30, mode};
    return f;
}

DispatchResult run_fan(const PreparedDefinition& def, std::uint8_t src_ep,
                       std::uint8_t mode) {
    const FanFrame ff = fan_mode(mode);
    auto raw = make_frame(0x0202 /* hvacFanCtrl */, src_ep,
                          std::span<const std::uint8_t>(ff.bytes.data(),
                                                        ff.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacFanCtrl";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

}  // namespace

// ── exposes: z2m fan + three channels; bogus bare fan_mode gone ──────
static void test_exposes() {
    const auto& def = devices::multiterm::kDef_ZC0101;
    assert(def_exposes(def, "fan_state"));
    assert(def_exposes(def, "silent_mode"));
    assert(def_exposes(def, "heating_cooling"));
    assert(def_exposes(def, "electric_valve"));
    // The auto-port's bare `fan_mode` Binary expose must be gone.
    assert(!def_exposes(def, "fan_mode"));
}

// ── genBinaryOutput presentValue decodes + endpoint suffixing ────────
static void test_binary_output_channels() {
    const auto& def = devices::multiterm::kDef_ZC0101;

    // ep8 (silent_mode) is unmapped -> bare `state`.
    auto ep8_on = run_bin_out(def, 8, 1);
    assert(ep8_on.any_matched);
    assert(b_true(ep8_on.merged.find("state")));
    assert(ep8_on.merged.find("state_8") == nullptr);

    auto ep8_off = run_bin_out(def, 8, 0);
    assert(ep8_off.any_matched);
    assert(b_false(ep8_off.merged.find("state")));

    // ep9 (heating_cooling) -> state_9.
    auto ep9 = run_bin_out(def, 9, 1);
    assert(ep9.any_matched);
    assert(b_true(ep9.merged.find("state_9")));
    assert(ep9.merged.find("state") == nullptr);  // bare key was suffixed

    // ep10 (electric_valve) -> state_10.
    auto ep10 = run_bin_out(def, 10, 0);
    assert(ep10.any_matched);
    assert(b_false(ep10.merged.find("state_10")));
    assert(ep10.merged.find("state") == nullptr);
}

// ── hvacFanCtrl fan_mode on ep8 stays unsuffixed (not state-collided) ─
static void test_fan_mode_unsuffixed() {
    const auto& def = devices::multiterm::kDef_ZC0101;

    auto r = run_fan(def, 8, 0x03 /* arbitrary fan mode */);
    assert(r.any_matched);
    const Value* fm = r.merged.find("fan_mode");
    assert(fm && fm->type == ValueType::Uint && fm->u == 0x03);
    // ep8 is unmapped, so the key must NOT be rewritten to fan_mode_8.
    assert(r.merged.find("fan_mode_8") == nullptr);
}

// ── binding coverage: hvacFanCtrl + genBinaryOutput on 8/9/10 ────────
static void test_bindings() {
    const auto& def = devices::multiterm::kDef_ZC0101;
    bool fan_ep8 = false, bo8 = false, bo9 = false, bo10 = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        const auto& b = def.bindings[i];
        if (b.endpoint == 8 && b.cluster_id == 0x0202) fan_ep8 = true;
        if (b.endpoint == 8 && b.cluster_id == 0x0010) bo8 = true;
        if (b.endpoint == 9 && b.cluster_id == 0x0010) bo9 = true;
        if (b.endpoint == 10 && b.cluster_id == 0x0010) bo10 = true;
    }
    assert(fan_ep8);
    assert(bo8);
    assert(bo9);
    assert(bo10);
}

int main() {
    test_exposes();
    test_binary_output_channels();
    test_fan_mode_unsuffixed();
    test_bindings();
    return 0;
}
