// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo AM25.
// Smart blind controller
// z2m-source: orvibo.ts #AM25.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_AM25[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCoverPosition,
};
const TzConverter* const kTz_AM25[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kModels_AM25[] = { "2a103244da0b406fa51410c692f79ead" };

}  // namespace

constexpr const char* kEnum_AM25_3[] = { "OPEN", "CLOSE", "STOP" };

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kEnum_AM25_3, 3},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0102},
};

extern const PreparedDefinition kDef_AM25{
    .zigbee_models=kModels_AM25, .zigbee_models_count=sizeof(kModels_AM25)/sizeof(kModels_AM25[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AM25", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AM25, .from_zigbee_count=sizeof(kFz_AM25)/sizeof(kFz_AM25[0]),
    .to_zigbee=kTz_AM25, .to_zigbee_count=sizeof(kTz_AM25)/sizeof(kTz_AM25[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
