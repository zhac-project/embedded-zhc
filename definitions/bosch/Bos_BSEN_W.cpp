// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch BSEN-W — Water alarm (formerly BWA-1, RBSH-WS-ZB-EU).
// Was mis-classed as a Light (state/brightness/color_temp) and decoded
// nothing the device sends. Re-targeted to the bosch/_shared typed-water
// bundle: genPowerCfg battery + ssIasZone → semantic `water_leak` key
// (zoneStatus bit 0), matching z2m's `waterAndTamperAlarm`.
// Gaps vs z2m: built-in siren (alarm_state / warning on ssIasWd) +
// alarm-on-motion are manuSpec/command surfaces — see BOSCH_PARITY.md.
// z2m-source: bosch.ts #BSEN-W.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {


constexpr const char* kModels_BSEN_W[] = { "RBSH-WS-ZB-EU" };

}  // namespace

extern const PreparedDefinition kDef_BSEN_W{
    .zigbee_models=kModels_BSEN_W, .zigbee_models_count=sizeof(kModels_BSEN_W)/sizeof(kModels_BSEN_W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BSEN-W", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschWaterLeak, .exposes_count=kExposesBoschWaterLeakCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschWaterLeak, .from_zigbee_count=kFzBoschWaterLeakCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschIasBattery,
    .bindings_count           = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
