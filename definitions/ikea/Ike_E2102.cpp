// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ikea E2102 — PRAKTLYSING cellular blind.
//
// z2m-source: ikea.ts #E2102 — m.windowCovering({controls:["lift"]}) +
// ikeaBattery() + ikeaConfigureGenPollCtrl() + m.identify().
//
// Rechargeable-battery cellular blind. Auto-generated def dropped
// ikeaBattery() (genPowerCfg) — battery level was dead. Added the
// generic battery decoder + expose + genPowerCfg binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {
const FzConverter* const kFz_E2102[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_E2102[] = {
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kModels_E2102[] = { "PRAKTLYSING cellular blind" };

constexpr Expose kExposes_E2102[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"battery",  ExposeType::Numeric, Access::State,    "%", nullptr, nullptr, 0},
    {"voltage",  ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_E2102[] = {
    {1, 0x0102},   // closuresWindowCovering
    {1, 0x0001},   // genPowerCfg
};

}  // namespace

extern const PreparedDefinition kDef_E2102{
    .zigbee_models=kModels_E2102, .zigbee_models_count=sizeof(kModels_E2102)/sizeof(kModels_E2102[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E2102", .vendor="Ikea",
    .meta=nullptr, .exposes=kExposes_E2102, .exposes_count=sizeof(kExposes_E2102)/sizeof(kExposes_E2102[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E2102, .from_zigbee_count=sizeof(kFz_E2102)/sizeof(kFz_E2102[0]),
    .to_zigbee=kTz_E2102, .to_zigbee_count=sizeof(kTz_E2102)/sizeof(kTz_E2102[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E2102, .bindings_count=sizeof(kBindings_E2102)/sizeof(kBindings_E2102[0]),
};

}  // namespace zhc::devices::ikea
