// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch ISW-ZPR1-WP13 — Motion sensor.
// Wires the bosch/_shared typed-motion bundle: genPowerCfg battery +
// ssIasZone decoded to the semantic `occupancy` key (zoneStatus bit 0),
// matching z2m. The auto-generated port emitted a bare `alarm` (generic
// kFzIasZone) that the SPA can't map to occupancy; switched to
// kFzBoschMotion.
// z2m-source: bosch.ts #ISW-ZPR1-WP13.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {
constexpr const char* kModels_ISW_ZPR1_WP13[] = { "ISW-ZPR1-WP13" };
}  // namespace

extern const PreparedDefinition kDef_ISW_ZPR1_WP13{
    .zigbee_models=kModels_ISW_ZPR1_WP13, .zigbee_models_count=sizeof(kModels_ISW_ZPR1_WP13)/sizeof(kModels_ISW_ZPR1_WP13[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ISW-ZPR1-WP13", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschMotion, .exposes_count=kExposesBoschMotionCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschMotion, .from_zigbee_count=kFzBoschMotionCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschIasBattery,
    .bindings_count           = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
