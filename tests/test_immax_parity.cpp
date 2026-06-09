// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Immax (Neo) IAS-ACE key fob.
// z2m-source: immax.ts #07046L — "4-Touch single click buttons":
//   fromZigbee: [fz.command_arm, fz.command_panic],
//   exposes:    e.action(["disarm", "arm_stay", "arm_away", "panic"]).
//
// Bug fixed: the generated 07046L def lowered kFzCommandRecall (genScenes
// cluster 0x0005, commandRecall → "recall_<scene>") and bound 0x0005.
// That decoder can NEVER fire for this device: a 4-Touch key fob sends
// IAS ACE (ssIasAce 0x0501) commandArm (0x00) and commandPanic (0x04),
// not scene recalls — so every button press produced no `action` state.
// The def was graduated to a Tier-2 parent override that swaps in the
// generic kFzIasAceArm + kFzIasAcePanic decoders (the direct equivalents
// of fz.command_arm / fz.command_panic) and binds ssIasAce (0x0501).
//
// These tests pin, on real ssIasAce command wire shapes, that the device
// now decodes `action` for arm + panic (and NOT a stale "recall_*"
// channel), and that it binds 0x0501 rather than 0x0005.

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

namespace zhc::devices::immax {
extern const PreparedDefinition kDef_D07046L;  // keyfob — ssIasAce arm / panic
}  // namespace zhc::devices::immax

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// Dispatch a cluster-specific ClientToServer command frame against `def`.
DispatchResult dispatch_cmd(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, /*src_ep=*/1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ssIasAce commandArm (0x00). ZCL header: fc=0x01 (cluster-specific,
// client->server, no mfr code), tsn, cmd=0x00. Body: arm_mode(1),
// code octstr (len u8 + bytes), zone_id(1).
std::vector<std::uint8_t> ace_arm(std::uint8_t arm_mode,
                                  std::span<const std::uint8_t> code = {},
                                  std::uint8_t zone_id = 0) {
    std::vector<std::uint8_t> v{0x01, 0x42, 0x00};
    v.push_back(arm_mode);
    v.push_back(static_cast<std::uint8_t>(code.size()));
    for (auto b : code) v.push_back(b);
    v.push_back(zone_id);
    return v;
}

// ssIasAce commandPanic (0x04). Header only, no body.
std::vector<std::uint8_t> ace_panic() {
    return std::vector<std::uint8_t>{0x01, 0x42, 0x04};
}

const char* action_of(const DispatchResult& r) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

// arm_mode 0 → "disarm" (the device's e.action lists disarm/arm_stay/
// arm_away/panic; z2m fz.command_arm emits the standard ACE arm-mode LUT,
// which is what kFzIasAceArm mirrors).
void test_disarm() {
    const auto& def = devices::immax::kDef_D07046L;
    auto r = dispatch_cmd(def, 0x0501, "ssIasAce", ace_arm(/*arm_mode=*/0));
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "disarm") == 0);
    // Must NOT decode the stale scene-recall channel.
    assert(r.merged.find("recall") == nullptr);
}

// arm_mode 3 → "arm_all_zones" (away). A non-empty PIN code is carried
// through as action_code; the trailing zone_id is honoured.
void test_arm_away_with_code() {
    const auto& def = devices::immax::kDef_D07046L;
    const std::uint8_t code[] = {'1', '2', '3', '4'};
    auto r = dispatch_cmd(def, 0x0501, "ssIasAce",
                          ace_arm(/*arm_mode=*/3, code, /*zone_id=*/0));
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "arm_all_zones") == 0);
    const Value* c = r.merged.find("action_code");
    assert(c && c->type == ValueType::StringRef && std::strcmp(c->str, "1234") == 0);
}

// commandPanic → action = "panic".
void test_panic() {
    const auto& def = devices::immax::kDef_D07046L;
    auto r = dispatch_cmd(def, 0x0501, "ssIasAce", ace_panic());
    assert(r.any_matched);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "panic") == 0);
}

// The def must bind ssIasAce (0x0501) and NOT genScenes (0x0005).
void test_bindings() {
    const auto& def = devices::immax::kDef_D07046L;
    assert(def_binds(def, 0x0501));
    assert(!def_binds(def, 0x0005));
}

}  // namespace

int main() {
    test_disarm();
    test_arm_away_with_code();
    test_panic();
    test_bindings();
    return 0;
}
