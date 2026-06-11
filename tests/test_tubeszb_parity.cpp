// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the TubesZB "tubeszb.router" (CC2652 Router). Pins the gap
// fixed when the def was graduated out of generated/:
//
//   * z2m defines this as a PURE router — fromZigbee: [fz.linkquality_from_basic],
//     toZigbee: [], exposes: []. Its only configure() binds genBasic (0x0000)
//     on endpoint 8 and configures zclVersion reporting.
//   * The auto-generated Tier-1 port carried a PHANTOM on/off bundle: a
//     kFzOnOff/kTzOnOff converter pair, a "state" Binary expose, and a bind to
//     genOnOff (0x0006) on endpoint 1. A CC2652 router is a mains-powered
//     repeater with no controllable on/off endpoint, so these were dead keys
//     (a phantom toggle in the shadow). The fix strips them to match z2m.
//   * z2m fingerprints on zigbeeModel "tubeszb.router" alone (no manufacturer
//     gate), so manufacturer_names stays null.
//
// z2m-source: zigbee-herdsman-converters/src/devices/tubeszb.ts.

#include <cassert>
#include <cstddef>
#include <cstring>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tubeszb {
extern const PreparedDefinition kDef_tubeszb_router;
}  // namespace zhc::devices::tubeszb

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

}  // namespace

int main() {
    using namespace zhc::devices::tubeszb;
    const PreparedDefinition& def = kDef_tubeszb_router;

    // Identity matches z2m.
    assert(def.model && std::strcmp(def.model, "tubeszb.router") == 0);
    assert(def.vendor && std::strcmp(def.vendor, "Tubeszb") == 0);

    // z2m fingerprint: zigbeeModel ["tubeszb.router"], no manufacturer gate.
    assert(def.zigbee_models_count == 1);
    assert(std::strcmp(def.zigbee_models[0], "tubeszb.router") == 0);
    assert(def.manufacturer_names_count == 0);
    assert(def.manufacturer_name_prefix == nullptr);

    // Pure router: z2m exposes:[], fromZigbee carries only linkquality (no
    // device-level converter), toZigbee:[]. The phantom on/off bundle must be
    // gone — no "state" expose, no fz/tz converters.
    assert(def.exposes_count == 0);
    assert(def.from_zigbee_count == 0);
    assert(def.to_zigbee_count == 0);
    assert(!def_exposes(def, "state"));

    // Only the genBasic bind from z2m's configure() survives — on endpoint 8,
    // not the phantom genOnOff (0x0006) bind on endpoint 1.
    assert(def.bindings_count == 1);
    assert(def.bindings[0].endpoint == 8);
    assert(def.bindings[0].cluster_id == 0x0000);  // genBasic

    return 0;
}
