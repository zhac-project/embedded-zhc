// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Trust ZPIR-8000 — hand-rewritten for parity (occupancy key).
// Motion sensor. z2m fz.ias_occupancy_alarm_2 → reads zoneStatus bit 1
// (kFzIasMotionAlarm2) and emits `occupancy` + `tamper` + `battery_low`.
// z2m-source: trust.ts #ZPIR-8000.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::trust {
namespace {
const FzConverter* const kFz_ZPIR_8000[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm2,
};
constexpr const char* kModels_ZPIR_8000[] = { "VMS_ADUROLIGHT" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_ZPIR_8000{
    .zigbee_models=kModels_ZPIR_8000, .zigbee_models_count=sizeof(kModels_ZPIR_8000)/sizeof(kModels_ZPIR_8000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZPIR-8000", .vendor="Trust",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZPIR_8000, .from_zigbee_count=sizeof(kFz_ZPIR_8000)/sizeof(kFz_ZPIR_8000[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::trust
