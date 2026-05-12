// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Konke LH07321 — IAS water leak detector (typed).
// z2m-source: konke.ts #LH07321 (fz.ias_water_leak_alarm_1).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::konke {
namespace {
const FzConverter* const kFz_LH07321[] = {
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_LH07321[] = { "LH07321" };

constexpr Expose kAutoExposes[] = {
    {"water_leak",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_LH07321{
    .zigbee_models=kModels_LH07321, .zigbee_models_count=sizeof(kModels_LH07321)/sizeof(kModels_LH07321[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LH07321", .vendor="Konke",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LH07321, .from_zigbee_count=sizeof(kFz_LH07321)/sizeof(kFz_LH07321[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::konke
