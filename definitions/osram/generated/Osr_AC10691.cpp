// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AC10691 — auto-generated.
// Smart+ plug
// z2m-source: osram.ts #AC10691.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AC10691[] = { "Plug Z3" };

}  // namespace


constexpr WhiteLabel kWhiteLabels_AC10691[] = {
    {"LEDVANCE","AC10691"},
};
extern const PreparedDefinition kDef_AC10691{
    .zigbee_models=kModels_AC10691, .zigbee_models_count=sizeof(kModels_AC10691)/sizeof(kModels_AC10691[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC10691", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramOnOff, .exposes_count=kExposesOsramOnOffCount,
    .white_labels=kWhiteLabels_AC10691, .white_labels_count=sizeof(kWhiteLabels_AC10691)/sizeof(kWhiteLabels_AC10691[0]),
    .from_zigbee=kFzOsramOnOff, .from_zigbee_count=kFzOsramOnOffCount,
    .to_zigbee=kTzOsramOnOff, .to_zigbee_count=kTzOsramOnOffCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramOnOff,
    .bindings_count           = kBindingsOsramOnOffCount,
};

}  // namespace zhc::devices::osram
