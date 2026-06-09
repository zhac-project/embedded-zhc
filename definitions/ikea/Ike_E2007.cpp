// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ikea E2007 — STARKVIND air purifier.
//
// z2m-source: ikea.ts #E2007 — ikeaAirPurifier() (fan_state/fan_speed/
// fan_mode/pm25/air_quality/child_lock/led_enable/filter & device age/
// replace_filter), m.identify().
//
// The auto-generated def wired the full kFzIkeaLight bundle — a phantom
// light on an air purifier with no genOnOff/genLevelCtrl/genColorCtrl.
//
// Every functional channel lives on IKEA's manuSpecificIkeaAirPurifier
// manufacturer cluster (0xfc7d): particulateMatter25Measurement,
// fanMode, fanSpeed, childLock, controlPanelLight, filterRunTime,
// replaceFilter, deviceRunTime. There is no generic decoder for this
// manufacturer cluster yet, so the device is stripped to an empty
// definition (no phantom light) and full support is tracked as a
// separate infra ticket.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {

constexpr const char* kModels_E2007[] = { "STARKVIND Air purifier", "STARKVIND Air purifier table" };

}  // namespace


constexpr WhiteLabel kWhiteLabels_E2007[] = {
    {"IKEA","E2006"},
};
extern const PreparedDefinition kDef_E2007{
    .zigbee_models=kModels_E2007, .zigbee_models_count=sizeof(kModels_E2007)/sizeof(kModels_E2007[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E2007", .vendor="Ikea",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=kWhiteLabels_E2007, .white_labels_count=sizeof(kWhiteLabels_E2007)/sizeof(kWhiteLabels_E2007[0]),
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::ikea
