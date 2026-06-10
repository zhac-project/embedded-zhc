// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Espressif ESP32-C6/H2 range extender / router
// (model "ZigbeeRangeExtender", manufacturer "Espressif"). Pins the gap
// fixed when the def was graduated out of generated/:
//
//   * z2m defines this as a PURE router — fromZigbee: [], toZigbee: [],
//     exposes: []. Its only configure() binds genBasic (0x0000).
//   * The auto-generated Tier-1 port carried a PHANTOM bundle: kFzOnOff +
//     kFzBattery converters, "state" / "battery" / "voltage" exposes, and
//     bindings to genPowerCfg (0x0001) + genOnOff (0x0006). A range
//     extender is mains-powered with no controllable on/off endpoint, so
//     these were dead keys (a phantom toggle + phantom battery/voltage in
//     the shadow). The fix strips them to match z2m exactly.
//   * z2m fingerprints on {modelID, manufacturerName: "Espressif"}, so the
//     def must gate the manufacturer too (the Tier-1 port was model-only).
//
// z2m-source: zigbee-herdsman-converters/src/devices/espressif.ts.

#include <cassert>
#include <cstddef>
#include <cstring>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::espressif {
extern const PreparedDefinition kDef_Espressif_ZigbeeRangeExtender;
}  // namespace zhc::devices::espressif

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool gates_manufacturer(const PreparedDefinition& def, const char* manu) {
    for (std::uint8_t i = 0; i < def.manufacturer_names_count; ++i)
        if (def.manufacturer_names[i] && std::strcmp(def.manufacturer_names[i], manu) == 0)
            return true;
    return false;
}

}  // namespace

int main() {
    using namespace zhc::devices::espressif;
    const PreparedDefinition& def = kDef_Espressif_ZigbeeRangeExtender;

    // Identity matches z2m.
    assert(def.model && std::strcmp(def.model, "Espressif-ZigbeeRangeExtender") == 0);
    assert(def.vendor && std::strcmp(def.vendor, "Espressif") == 0);

    // z2m fingerprint: {modelID: "ZigbeeRangeExtender", manufacturerName: "Espressif"}.
    assert(def.zigbee_models_count == 1);
    assert(std::strcmp(def.zigbee_models[0], "ZigbeeRangeExtender") == 0);
    assert(def.manufacturer_names_count == 1);
    assert(gates_manufacturer(def, "Espressif"));

    // Pure router: z2m exposes:[], fromZigbee:[], toZigbee:[].
    // The phantom on/off + battery bundle must be gone.
    assert(def.exposes_count == 0);
    assert(def.from_zigbee_count == 0);
    assert(def.to_zigbee_count == 0);
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));

    // Only the genBasic bind from z2m's configure() survives — no genPowerCfg
    // (0x0001) or genOnOff (0x0006) phantom binds.
    assert(def.bindings_count == 1);
    assert(def.bindings[0].endpoint == 1);
    assert(def.bindings[0].cluster_id == 0x0000);  // genBasic

    return 0;
}
