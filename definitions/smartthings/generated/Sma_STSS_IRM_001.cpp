// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings STSS-IRM-001 — parity-extended 2026-04-28.
// Motion sensor (2013 model, original "PGC314").
// z2m-source: smartthings.ts #STSS-IRM-001.
//
// z2m bundle: IAS motion (alarm_1 → occupancy, battery_low, tamper) only.
// No battery/voltage/temperature in z2m.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_STSS_IRM_001[] = {
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_STSS_IRM_001[] = { "PGC314" };

constexpr Expose kAutoExposes[] = {
    {"occupancy",   ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDef_STSS_IRM_001{
    .zigbee_models=kModels_STSS_IRM_001, .zigbee_models_count=sizeof(kModels_STSS_IRM_001)/sizeof(kModels_STSS_IRM_001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STSS-IRM-001", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STSS_IRM_001, .from_zigbee_count=sizeof(kFz_STSS_IRM_001)/sizeof(kFz_STSS_IRM_001[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
