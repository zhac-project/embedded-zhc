// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZS1100400-IN-V1A02 — hand-rewritten 2026-04-28q.
// PIR motion sensor, wireless motion detector.
// z2m bundle: fz.ias_occupancy_alarm_1 + fz.battery → typed IAS.
// z2m-source: linkind.ts #ZS1100400-IN-V1A02.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::linkind {
namespace {
const FzConverter* const kFz_ZS1100400_IN_V1A02[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_ZS1100400_IN_V1A02[] = { "ZB-MotionSensor-D0003" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_ZS1100400_IN_V1A02{
    .zigbee_models=kModels_ZS1100400_IN_V1A02, .zigbee_models_count=sizeof(kModels_ZS1100400_IN_V1A02)/sizeof(kModels_ZS1100400_IN_V1A02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZS1100400-IN-V1A02", .vendor="Linkind",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZS1100400_IN_V1A02, .from_zigbee_count=sizeof(kFz_ZS1100400_IN_V1A02)/sizeof(kFz_ZS1100400_IN_V1A02[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::linkind
