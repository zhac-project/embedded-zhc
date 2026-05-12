// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Trust ZCTS-808 — hand-rewritten for parity (contact key).
// Wireless contact sensor. z2m fz.ias_contact → emits `contact` (bit 0
// — true == open, matching z2m).
// z2m-source: trust.ts #ZCTS-808.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::trust {
namespace {
const FzConverter* const kFz_ZCTS_808[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};
constexpr const char* kModels_ZCTS_808[] = { "CSW_ADUROLIGHT" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_ZCTS_808{
    .zigbee_models=kModels_ZCTS_808, .zigbee_models_count=sizeof(kModels_ZCTS_808)/sizeof(kModels_ZCTS_808[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZCTS-808", .vendor="Trust",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZCTS_808, .from_zigbee_count=sizeof(kFz_ZCTS_808)/sizeof(kFz_ZCTS_808[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::trust
