// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the LED-Trading family.
// z2m-source: led_trading.ts
//
// Bug fixed (found by a z2m<->embedded-zhc parity pass):
//   The 9133 "Pushbutton transmitter module" — a self-powered Green Power
//   transmitter (fingerprint modelID "GreenPower_2") — was mis-ported as a
//   *controllable* on/off relay: the generated def wired generic kFzOnOff +
//   kTzOnOff, declared a dead `state` Binary expose, and bound genOnOff
//   (0x0006) to the coordinator. In z2m the device is a pure action remote:
//   `toZigbee: []` plus a single `e.action([press_1, hold_1, … hold_4])`.
//   These tests pin that the def now exposes the semantic `action` enum (with
//   z2m's action list), carries NO phantom `state`, is not controllable
//   (to_zigbee empty), and binds nothing.
//
//   The action *decoder* itself is a documented INFRA defer: z2m decodes via
//   `cluster: "greenPower"` + `type: commandNotification`, a Green Power frame
//   family the embedded-zhc parser does not yet produce (mirrors the enocean
//   PTM 21x precedent). Asserting decode is out of scope.
//
// The remaining three LED-Trading defs were verified parity-correct and are
// pinned here as regression guards:
//   * 9134 powerstrip — 5-gang m.onOff, exposes bare `state`, carries the
//     l1..l5 endpoint_map so the runtime suffixes state_l1..state_l5.
//   * 9135 curtain controller — closuresWindowCovering position cover.
//   * HK-LN-DIM-A dimmer — m.light() => on/off + brightness, no color.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::led_trading {
extern const PreparedDefinition kDef_D9133;
extern const PreparedDefinition kDef_D9134;
extern const PreparedDefinition kDef_D9135;
extern const PreparedDefinition kDef_HK_LN_DIM_A;
}  // namespace zhc::devices::led_trading

using namespace zhc;

namespace {

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return &def.exposes[i];
    return nullptr;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    return find_expose(def, key) != nullptr;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool action_lists(const PreparedDefinition& def, const char* value) {
    const Expose* a = find_expose(def, "action");
    if (!a) return false;
    for (std::size_t i = 0; i < a->enum_count; ++i)
        if (a->enum_values[i] && std::strcmp(a->enum_values[i], value) == 0)
            return true;
    return false;
}

const EndpointLabel* find_endpoint(const PreparedDefinition& def, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i)
        if (def.endpoint_map[i].endpoint == ep) return &def.endpoint_map[i];
    return nullptr;
}

// ── 9133 — Green Power pushbutton, role parity ──────────────────────
void test_9133_action_remote() {
    const auto& def = devices::led_trading::kDef_D9133;
    const Expose* a = find_expose(def, "action");
    assert(a != nullptr);                  // semantic key present
    assert(a->type == ExposeType::Enum);   // enum, not Binary state
    assert(a->enum_count == 8);            // press_1..4 + hold_1..4
    assert(action_lists(def, "press_1"));
    assert(action_lists(def, "hold_1"));
    assert(action_lists(def, "press_4"));
    assert(action_lists(def, "hold_4"));
    assert(!action_lists(def, "on"));      // never an on/off relay action

    assert(!def_exposes(def, "state"));    // phantom relay removed
    assert(def.to_zigbee_count == 0);      // not controllable (z2m toZigbee:[])
    assert(def.from_zigbee_count == 0);    // decoder deferred (GP infra)
    assert(!def_binds(def, 0x0006));       // no genOnOff bind on a GP transmitter
    assert(def.bindings_count == 0);       // no phantom bindings at all
}

// ── 9134 — 5-gang powerstrip, endpoint_map drives state_l1..l5 ──────
void test_9134_multigang() {
    const auto& def = devices::led_trading::kDef_D9134;
    // Exposes stay bare; the runtime suffixes per endpoint_map.
    assert(def_exposes(def, "state"));
    assert(!def_exposes(def, "state_l1"));   // bare in schema, suffixed at runtime
    // endpoint_map must enumerate all five gangs so dispatch suffixes them.
    assert(def.endpoint_map_count == 5);
    for (std::uint8_t ep = 1; ep <= 5; ++ep) {
        const EndpointLabel* l = find_endpoint(def, ep);
        assert(l != nullptr);
        assert(l->label != nullptr);
    }
    // l1->1 .. l5->5 label mapping.
    assert(std::strcmp(find_endpoint(def, 1)->label, "l1") == 0);
    assert(std::strcmp(find_endpoint(def, 5)->label, "l5") == 0);
    assert(def.to_zigbee_count > 0);         // controllable relay bank
}

// ── 9135 — curtain controller, position cover ──────────────────────
void test_9135_cover() {
    const auto& def = devices::led_trading::kDef_D9135;
    assert(def_exposes(def, "position"));    // z2m e.cover_position()
    assert(!def_exposes(def, "tilt"));       // z2m exposes position only
    assert(!def_exposes(def, "state"));      // not an on/off device
    assert(def.from_zigbee_count > 0);       // cover position decoder wired
}

// ── HK-LN-DIM-A — dimmer, on/off + brightness, no color ─────────────
void test_dimmer() {
    const auto& def = devices::led_trading::kDef_HK_LN_DIM_A;
    assert(def_exposes(def, "state"));       // m.light() on/off
    assert(def_exposes(def, "brightness"));  // m.light() level
    assert(!def_exposes(def, "color_temp")); // plain m.light(), no color
    assert(!def_exposes(def, "color"));
    assert(def_binds(def, 0x0008));          // genLevelCtrl bound for brightness
}

}  // namespace

int main() {
    test_9133_action_remote();
    test_9134_multigang();
    test_9135_cover();
    test_dimmer();
    return 0;
}
