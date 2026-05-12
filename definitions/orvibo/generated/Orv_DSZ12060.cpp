// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo DSZ12060.
// Spot light S10
// z2m-source: orvibo.ts #DSZ12060.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_DSZ12060[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_DSZ12060[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_DSZ12060[] = { "9ff5a780c5a4470d9087175c71d50f92" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mireds", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_DSZ12060{
    .zigbee_models=kModels_DSZ12060, .zigbee_models_count=sizeof(kModels_DSZ12060)/sizeof(kModels_DSZ12060[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DSZ12060", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DSZ12060, .from_zigbee_count=sizeof(kFz_DSZ12060)/sizeof(kFz_DSZ12060[0]),
    .to_zigbee=kTz_DSZ12060, .to_zigbee_count=sizeof(kTz_DSZ12060)/sizeof(kTz_DSZ12060[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
