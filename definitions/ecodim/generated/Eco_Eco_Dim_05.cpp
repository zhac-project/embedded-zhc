// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecodim Eco-Dim.05 — duo dimmer (hand-corrected, 2026-04-28q).
// LED dimmer duo 2x 0-100W (2 endpoints).
// z2m-source: ecodim.ts #Eco-Dim.05.
//
// extend: deviceEndpoints({left: 2, right: 1}),
//         light({effect:false, configureReporting:true,
//                endpointNames:["left","right"]}).
//
// Notes:
//   - z2m maps EP2 -> left, EP1 -> right (counter-intuitive). Honour
//     the same so action keys produced via the per-EP dispatcher
//     match z2m output.
//   - The duo and the Eco-Dim.07/10 single-EP variant share modelID
//     "EcoDim-Zigbee 3.0". Disambiguating the two requires endpoint
//     topology, which ZHC's runtime can't see at lookup time. We keep
//     only the duo-specific zigbeeModel strings here; the bare
//     "EcoDim-Zigbee 3.0" id is left to Eco-Dim.07/10 to claim.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
const FzConverter* const kFz_Eco_Dim_05[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_Eco_Dim_05[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_Eco_Dim_05[] = { "Eco-Dim.05 Zigbee", "eco-dim05-zigbee" };

constexpr ::zhc::EndpointLabel kEndpoints_Eco_Dim_05[] = { {"left", 2}, {"right", 1} };

constexpr Expose kExposes_Eco_Dim_05[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_Eco_Dim_05[] = {
    {1, 0x0006}, {1, 0x0008},
    {2, 0x0006}, {2, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_Eco_Dim_05{
    .zigbee_models=kModels_Eco_Dim_05, .zigbee_models_count=sizeof(kModels_Eco_Dim_05)/sizeof(kModels_Eco_Dim_05[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Eco-Dim.05", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_Eco_Dim_05, .exposes_count=sizeof(kExposes_Eco_Dim_05)/sizeof(kExposes_Eco_Dim_05[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Eco_Dim_05, .from_zigbee_count=sizeof(kFz_Eco_Dim_05)/sizeof(kFz_Eco_Dim_05[0]),
    .to_zigbee=kTz_Eco_Dim_05, .to_zigbee_count=sizeof(kTz_Eco_Dim_05)/sizeof(kTz_Eco_Dim_05[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_Eco_Dim_05, .bindings_count=sizeof(kBindings_Eco_Dim_05)/sizeof(kBindings_Eco_Dim_05[0]),
    .endpoint_map       = kEndpoints_Eco_Dim_05,
    .endpoint_map_count = sizeof(kEndpoints_Eco_Dim_05)/sizeof(kEndpoints_Eco_Dim_05[0]),
};

}  // namespace zhc::devices::ecodim
