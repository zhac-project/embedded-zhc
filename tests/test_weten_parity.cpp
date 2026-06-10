// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for WETEN. The vendor has two z2m devices: a plain TS0001 touch
// switch (1GNNTS, m.onOff — at parity) and the TS0601 "PCI E" Tuya-DP remote
// control card. The generator emitted THREE colliding artifacts for the single
// PCI E device:
//
//   - a MISROUTED bare genOnOff def (cluster 0x0006) carrying both _TZE* names,
//   - two model-only DP defs split one-per-manufacturer-name.
//
// They were consolidated into one `kDef_PCI_E` carrying both manufacturer names
// (_TZE204_6fk3gewc + _TZE284_6fk3gewc), the "Weten / Tuya PRO" white-label and
// the real model "PCI E", wired to the 0xEF00 DP map (not genOnOff).
//
// The real decode bug: DP101 restart_mode lost two of its three enum values.
// z2m wires {restart:0, "force restart":1, "–":2}; the generator emitted only
// {0:"restart"}, so a "force restart" (1) or "–" (2) report silently dropped.
//
// These tests resolve both manufacturer names to the consolidated def and
// decode real 0xEF00 (manuSpecificTuya) DP frames, asserting the corrected
// restart_mode labels and that the device routes through the DP map.
//
// z2m-source: zigbee-herdsman-converters/src/devices/weten.ts.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/weten_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> weten_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::weten::kWetenRegistry,
        devices::weten::kWetenRegistryCount);
}

DecodedMessage make_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

InboundApsFrame make_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

const PreparedDefinition& resolve(const char* manu) {
    const auto* d = find_definition("TS0601", manu, weten_registry());
    assert(d && "weten manufacturer name must resolve to a def");
    return *d;
}

// Decode a single Enum DP (dp_type 0x04, 1 wire byte) and return the StringRef
// label emitted under out_key (nullptr if absent / not a string).
const char* decode_enum(const PreparedDefinition& def, std::uint8_t dp,
                        std::uint8_t wire, const char* out_key) {
    const std::uint8_t b[] = { wire };
    const TuyaDpRecord recs[] = {
        { dp, 0x04, std::span<const std::uint8_t>(b, 1) },
    };
    const auto result = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find(out_key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

}  // namespace

// --- Both manufacturer names resolve to the single consolidated "PCI E" def --
// (regression guard for the de-split: a model-only TS0601 def would NOT have
//  matched a manufacturer name, and the misrouted genOnOff dup must be gone.)
static void test_both_mfr_names_resolve_to_pci_e() {
    const auto& a = resolve("_TZE204_6fk3gewc");
    assert(a.model && std::strcmp(a.model, "PCI E") == 0);
    const auto& b = resolve("_TZE284_6fk3gewc");
    assert(b.model && std::strcmp(b.model, "PCI E") == 0);
    // Same def object for both names.
    assert(&a == &b);
    // White-label carried over.
    assert(a.white_labels && a.white_labels_count == 1);
    assert(std::strcmp(a.white_labels[0].model, "Tuya PRO") == 0);
}

// --- The device routes through the 0xEF00 DP map, not genOnOff. -------------
// DP1 (Bool) -> state. The misrouted dup wired kFzOnOff (cluster 0x0006) which
// would NOT match a TuyaDp-family frame, so any_matched would be false.
static void test_state_dp_routes_through_tuya_map() {
    const auto& d = resolve("_TZE204_6fk3gewc");
    const std::uint8_t on[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 1, 0x01, std::span<const std::uint8_t>(on, 1) },
    };
    const auto result = dispatch_dp(d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched && "DP1 must route through the Tuya DP map");
    const Value* v = result.merged.find("state");
    assert(v && "DP1 must surface state");
}

// --- Bug fix: DP101 restart_mode keeps all three enum values ----------------
// z2m {restart:0, "force restart":1, "–":2}. The generator dropped 1 and 2.
static void test_restart_mode_full_enum() {
    const auto& d = resolve("_TZE204_6fk3gewc");

    const char* v0 = decode_enum(d, 101, 0, "restart_mode");
    assert(v0 && std::strcmp(v0, "restart") == 0);

    // Previously dropped (unmapped -> no StringRef emitted).
    const char* v1 = decode_enum(d, 101, 1, "restart_mode");
    assert(v1 && std::strcmp(v1, "force restart") == 0);

    const char* v2 = decode_enum(d, 101, 2, "restart_mode");
    assert(v2 && std::strcmp(v2, "\xe2\x80\x93") == 0);  // "–" U+2013
}

// --- Regression guard: the other enums kept their (correct) order -----------
static void test_other_enums_unchanged() {
    const auto& d = resolve("_TZE284_6fk3gewc");

    const char* rf0 = decode_enum(d, 102, 0, "rf_remote_control");
    assert(rf0 && std::strcmp(rf0, "ON") == 0);
    const char* rf1 = decode_enum(d, 102, 1, "rf_remote_control");
    assert(rf1 && std::strcmp(rf1, "OFF") == 0);

    const char* po0 = decode_enum(d, 105, 0, "power_on_behavior");
    assert(po0 && std::strcmp(po0, "off") == 0);
    const char* po1 = decode_enum(d, 105, 1, "power_on_behavior");
    assert(po1 && std::strcmp(po1, "on") == 0);
}

int main() {
    test_both_mfr_names_resolve_to_pci_e();
    test_state_dp_routes_through_tuya_map();
    test_restart_mode_full_enum();
    test_other_enums_unchanged();
    return 0;
}
