// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the EnOcean PTM 215Z / 215ZE / 216Z family of
// self-powered Green Power pushbutton transmitters.
// z2m-source: enocean.ts (fz.enocean_ptm215z / ptm215ze / ptm216z).
//
// Bug fixed (found by a z2m<->embedded-zhc parity pass):
//   All three PTM transmitters were mis-ported as *controllable* on/off
//   relays — the generated defs wired generic kFzOnOff + kTzOnOff, declared
//   a dead `state` Binary expose, and bound genOnOff (0x0006) to the
//   coordinator. In z2m each device is a pure action remote: `toZigbee: []`
//   plus a single `e.action([...])`. These tests pin that each def now
//   exposes the semantic `action` enum (with z2m's action list), carries NO
//   phantom `state`, is not a controllable sink (to_zigbee empty), and binds
//   no genOnOff cluster.
//
// The action *decoder* itself is a documented INFRA defer: z2m decodes via
// `cluster: "greenPower"` + `type: commandNotification`, a Green Power frame
// family the embedded-zhc parser does not yet produce. Asserting decode is
// out of scope; the schema/role parity is what the shadow + SPA consume.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::enocean {
extern const PreparedDefinition kDef_PTM_215Z;
extern const PreparedDefinition kDef_PTM_215ZE;
extern const PreparedDefinition kDef_PTM_216Z;
}  // namespace zhc::devices::enocean

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

// Every PTM transmitter shares the same role-parity shape: action enum,
// no phantom relay, no controllable sink, no genOnOff bind.
void check_action_remote(const PreparedDefinition& def, std::size_t expected_actions) {
    const Expose* a = find_expose(def, "action");
    assert(a != nullptr);                       // semantic key present
    assert(a->type == ExposeType::Enum);        // enum, not Binary state
    assert(a->enum_count == expected_actions);  // full z2m action list
    assert(!def_exposes(def, "state"));         // phantom relay removed
    assert(def.to_zigbee_count == 0);           // not controllable (z2m toZigbee:[])
    assert(def.from_zigbee_count == 0);         // decoder deferred (GP infra)
    assert(!def_binds(def, 0x0006));            // no genOnOff bind on a GP transmitter
    assert(def.bindings_count == 0);            // no phantom bindings at all
}

void test_ptm215z() {
    const auto& def = devices::enocean::kDef_PTM_215Z;
    check_action_remote(def, 13);
    assert(action_lists(def, "press_1"));
    assert(action_lists(def, "release_4"));
    assert(action_lists(def, "press_1_and_3"));
    assert(action_lists(def, "press_energy_bar"));
    assert(!action_lists(def, "on"));   // never an on/off relay action
}

void test_ptm215ze() {
    const auto& def = devices::enocean::kDef_PTM_215ZE;
    check_action_remote(def, 27);
    assert(action_lists(def, "press_or_release_all"));
    assert(action_lists(def, "lock"));
    assert(action_lists(def, "tilt"));
    assert(action_lists(def, "release_energy_bar"));
}

void test_ptm216z() {
    const auto& def = devices::enocean::kDef_PTM_216Z;
    check_action_remote(def, 18);
    assert(action_lists(def, "press_1_and_2_and_3"));
    assert(action_lists(def, "press_all"));
    assert(action_lists(def, "release"));
    assert(action_lists(def, "short_press_2_of_2"));
}

}  // namespace

int main() {
    test_ptm215z();
    test_ptm215ze();
    test_ptm216z();
    return 0;
}
