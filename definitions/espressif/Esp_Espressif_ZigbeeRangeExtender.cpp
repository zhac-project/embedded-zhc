// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Espressif Espressif-ZigbeeRangeExtender — graduated from generated/.
// ESP32-C6/H2 router / range extender.
//
// z2m-source: espressif.ts #Espressif-ZigbeeRangeExtender.
//   The z2m definition is a pure router: fromZigbee: [], toZigbee: [],
//   exposes: []. Its only configure() binds genBasic to report zclVersion.
//   The auto-generated Tier-1 port carried a PHANTOM bundle: kFzOnOff +
//   kFzBattery converters, "state"/"battery"/"voltage" exposes, and
//   bindings to genPowerCfg (0x0001) + genOnOff (0x0006) — none of which
//   exist in z2m. A range extender is mains-powered and has no controllable
//   on/off endpoint, so those were dead keys (a phantom toggle + phantom
//   battery in the shadow). Stripped to match z2m: no exposes, no fz/tz,
//   only the genBasic bind from z2m's configure(). z2m fingerprints on
//   {modelID, manufacturerName: "Espressif"}, so gate the manufacturer too.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::espressif {
namespace {


constexpr const char* kModels_Espressif_ZigbeeRangeExtender[] = { "ZigbeeRangeExtender" };
constexpr const char* kMfrs_Espressif_ZigbeeRangeExtender[]   = { "Espressif" };

// z2m configure(): reporting.bind(endpoint, coordinatorEndpoint, ["genBasic"]).
constexpr BindingSpec kBind_Espressif_ZigbeeRangeExtender[] = {
    { 1, 0x0000 },  // genBasic
};

}  // namespace

extern const PreparedDefinition kDef_Espressif_ZigbeeRangeExtender{
    .zigbee_models=kModels_Espressif_ZigbeeRangeExtender, .zigbee_models_count=sizeof(kModels_Espressif_ZigbeeRangeExtender)/sizeof(kModels_Espressif_ZigbeeRangeExtender[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kMfrs_Espressif_ZigbeeRangeExtender, .manufacturer_names_count=sizeof(kMfrs_Espressif_ZigbeeRangeExtender)/sizeof(kMfrs_Espressif_ZigbeeRangeExtender[0]),
    .model="Espressif-ZigbeeRangeExtender", .vendor="Espressif",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_Espressif_ZigbeeRangeExtender,
    .bindings_count           = sizeof(kBind_Espressif_ZigbeeRangeExtender)/sizeof(kBind_Espressif_ZigbeeRangeExtender[0]),
};

}  // namespace zhc::devices::espressif
