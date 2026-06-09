// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch RADION TriTech ZB — Wireless motion detector.
// Wires the bosch/_shared typed-motion bundle: genPowerCfg battery +
// ssIasZone decoded to the semantic `occupancy` key (zoneStatus bit 0),
// matching z2m. The auto-generated port emitted a bare `alarm` (generic
// kFzIasZone) that the SPA can't map to occupancy; switched to
// kFzBoschMotion.
// z2m-source: bosch.ts #RADION TriTech ZB.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {
constexpr const char* kModels_RADION_TriTech_ZB[] = { "RFDL-ZB", "RFDL-ZB-EU", "RFDL-ZB-H", "RFDL-ZB-K", "RFDL-ZB-CHI", "RFDL-ZB-MS", "RFDL-ZB-ES", "RFPR-ZB", "RFPR-ZB-EU", "RFPR-ZB-CHI", "RFPR-ZB-ES", "RFPR-ZB-MS" };
}  // namespace

extern const PreparedDefinition kDef_RADION_TriTech_ZB{
    .zigbee_models=kModels_RADION_TriTech_ZB, .zigbee_models_count=sizeof(kModels_RADION_TriTech_ZB)/sizeof(kModels_RADION_TriTech_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RADION TriTech ZB", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschMotion, .exposes_count=kExposesBoschMotionCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschMotion, .from_zigbee_count=kFzBoschMotionCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschIasBattery,
    .bindings_count           = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
