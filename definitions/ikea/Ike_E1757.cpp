// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ikea E1757 — FYRTUR roller blind, block-out.
//
// z2m-source: ikea.ts #E1757 — m.windowCovering({controls:["lift"]}) +
// ikeaBattery() + ikeaConfigureGenPollCtrl() + m.identify().
//
// FYRTUR is a rechargeable-battery roller blind. The auto-generated def
// wired only the cover position; ikeaBattery() (genPowerCfg) was
// dropped, leaving the battery level dead. Added the generic battery
// decoder + expose + genPowerCfg binding to match z2m.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {
const FzConverter* const kFz_E1757[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_E1757[] = {
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kModels_E1757[] = { "FYRTUR block-out roller blind" };

constexpr Expose kExposes_E1757[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"battery",  ExposeType::Numeric, Access::State,    "%", nullptr, nullptr, 0},
    {"voltage",  ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_E1757[] = {
    {1, 0x0102},   // closuresWindowCovering
    {1, 0x0001},   // genPowerCfg
};

}  // namespace

extern const PreparedDefinition kDef_E1757{
    .zigbee_models=kModels_E1757, .zigbee_models_count=sizeof(kModels_E1757)/sizeof(kModels_E1757[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E1757", .vendor="Ikea",
    .meta=nullptr, .exposes=kExposes_E1757, .exposes_count=sizeof(kExposes_E1757)/sizeof(kExposes_E1757[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E1757, .from_zigbee_count=sizeof(kFz_E1757)/sizeof(kFz_E1757[0]),
    .to_zigbee=kTz_E1757, .to_zigbee_count=sizeof(kTz_E1757)/sizeof(kTz_E1757[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E1757, .bindings_count=sizeof(kBindings_E1757)/sizeof(kBindings_E1757[0]),
};

}  // namespace zhc::devices::ikea
