// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo SM10ZW.
// Door or window contact switch
// z2m-source: orvibo.ts #SM10ZW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_SM10ZW[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};
constexpr const char* kModels_SM10ZW[] = { "fdd76effa0e146b4bdafa0c203a37192", "c670e231d1374dbc9e3c6a9fffbd0ae6", "75a4bfe8ef9c4350830a25d13e3ab068" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_SM10ZW{
    .zigbee_models=kModels_SM10ZW, .zigbee_models_count=sizeof(kModels_SM10ZW)/sizeof(kModels_SM10ZW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SM10ZW", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SM10ZW, .from_zigbee_count=sizeof(kFz_SM10ZW)/sizeof(kFz_SM10ZW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
