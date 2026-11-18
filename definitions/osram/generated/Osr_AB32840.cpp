// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AB32840 — auto-generated.
// LIGHTIFY LED Classic B40 tunable white
// z2m-source: osram.ts #AB32840.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


// z2m v26.129.0 (#13717): + ledvancePowerOnBehavior(), so own tz / exposes (bundle CTLight + power_on_behavior).
const TzConverter* const kTz_AB32840[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &kTzOsramPowerOnBehavior,
};
constexpr Expose kExposes_AB32840[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kOsramPowerOnBehaviorValues, std::size(kOsramPowerOnBehaviorValues)},
};
constexpr const char* kModels_AB32840[] = { "Classic B40 TW - LIGHTIFY" };

}  // namespace

extern const PreparedDefinition kDef_AB32840{
    .zigbee_models=kModels_AB32840, .zigbee_models_count=sizeof(kModels_AB32840)/sizeof(kModels_AB32840[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AB32840", .vendor="Osram",
    .meta=nullptr, .exposes=kExposes_AB32840, .exposes_count=sizeof(kExposes_AB32840)/sizeof(kExposes_AB32840[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramCTLight, .from_zigbee_count=kFzOsramCTLightCount,
    .to_zigbee=kTz_AB32840, .to_zigbee_count=sizeof(kTz_AB32840)/sizeof(kTz_AB32840[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramCTLight,
    .bindings_count           = kBindingsOsramCTLightCount,
};

}  // namespace zhc::devices::osram
