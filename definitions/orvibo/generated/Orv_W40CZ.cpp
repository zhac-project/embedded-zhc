// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo W40CZ.
// Smart curtain motor
// z2m-source: orvibo.ts #W40CZ.
// z2m also pulls fz.curtain_position_analog_output (genAnalogInput 0x0055)
// — ZHC has no equivalent; position is only reported via 0x0102
// currentPositionLiftPercentage.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_W40CZ[] = {
    &::zhc::generic::kFzCoverPosition,
};
const TzConverter* const kTz_W40CZ[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kModels_W40CZ[] = { "093199ff04984948b4c78167c8e7f47e", "c8daea86aa9c415aa524365775b1218c", "c8daea86aa9c415aa524365775b1218" };

}  // namespace

constexpr const char* kEnum_W40CZ_1[] = { "OPEN", "CLOSE", "STOP" };

constexpr Expose kAutoExposes[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kEnum_W40CZ_1, 3},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},
};

extern const PreparedDefinition kDef_W40CZ{
    .zigbee_models=kModels_W40CZ, .zigbee_models_count=sizeof(kModels_W40CZ)/sizeof(kModels_W40CZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="W40CZ", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_W40CZ, .from_zigbee_count=sizeof(kFz_W40CZ)/sizeof(kFz_W40CZ[0]),
    .to_zigbee=kTz_W40CZ, .to_zigbee_count=sizeof(kTz_W40CZ)/sizeof(kTz_W40CZ[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
