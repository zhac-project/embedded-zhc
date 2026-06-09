// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch BSIR-EZ — Outdoor siren (RBSH-OS-ZB-EU).
// Was mis-classed as a Light (state/brightness/color_temp) and decoded
// nothing the device sends. Re-targeted to the bosch/_shared IAS-zone
// battery bundle: genPowerCfg battery + ssIasZone (zoneStatus bit 0 →
// `alarm`, bit 2 → tamper, bit 3 → battery_low), matching z2m's
// `iasZoneStatus` surface for this siren. `alarm` is the right semantic
// key for a siren's alarm-active state (not a contact/smoke/water type).
// Gaps vs z2m (INFRA, deferred): the siren command/config surface
// (alarm_control / alarm_mode / siren_volume / siren_duration / light_*)
// rides Bosch's custom alarmControl command on ssIasZone (mfgcode 0x1209)
// + solar/power-source attrs — no generic converter exists. See
// docs/BOSCH_PARITY.md.
// z2m-source: bosch.ts #BSIR-EZ.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {


constexpr const char* kModels_BSIR_EZ[] = { "RBSH-OS-ZB-EU" };

}  // namespace

extern const PreparedDefinition kDef_BSIR_EZ{
    .zigbee_models=kModels_BSIR_EZ, .zigbee_models_count=sizeof(kModels_BSIR_EZ)/sizeof(kModels_BSIR_EZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BSIR-EZ", .vendor="Bosch",
    .meta=nullptr, .exposes=kExposesBoschIasBattery, .exposes_count=kExposesBoschIasBatteryCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzBoschIasBattery, .from_zigbee_count=kFzBoschIasBatteryCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsBoschIasBattery,
    .bindings_count           = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
