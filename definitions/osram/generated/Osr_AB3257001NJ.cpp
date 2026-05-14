// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AB3257001NJ — auto-generated.
// Smart+ plug
// z2m-source: osram.ts #AB3257001NJ.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AB3257001NJ[] = { "Plug 01" };

}  // namespace


constexpr WhiteLabel kWhiteLabels_AB3257001NJ[] = {
    {"LEDVANCE","AB3257001NJ"},
    {"LEDVANCE","AC03360"},
};
extern const PreparedDefinition kDef_AB3257001NJ{
    .zigbee_models=kModels_AB3257001NJ, .zigbee_models_count=sizeof(kModels_AB3257001NJ)/sizeof(kModels_AB3257001NJ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AB3257001NJ", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramOnOff, .exposes_count=kExposesOsramOnOffCount,
    .white_labels=kWhiteLabels_AB3257001NJ, .white_labels_count=sizeof(kWhiteLabels_AB3257001NJ)/sizeof(kWhiteLabels_AB3257001NJ[0]),
    .from_zigbee=kFzOsramOnOff, .from_zigbee_count=kFzOsramOnOffCount,
    .to_zigbee=kTzOsramOnOff, .to_zigbee_count=kTzOsramOnOffCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramOnOff,
    .bindings_count           = kBindingsOsramOnOffCount,
};

}  // namespace zhc::devices::osram
