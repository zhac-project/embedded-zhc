// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Trust ZWLD-100 — hand-rewritten for parity (water_leak key).
// Water leakage detector. z2m fz.ias_water_leak_alarm_1 → emits
// `water_leak` (bit 0) + `tamper` + `battery_low`.
// z2m-source: trust.ts #ZWLD-100.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::trust {
namespace {
const FzConverter* const kFz_ZWLD_100[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};
constexpr const char* kModels_ZWLD_100[] = { "WATER_TPV14" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"water_leak",  ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_ZWLD_100{
    .zigbee_models=kModels_ZWLD_100, .zigbee_models_count=sizeof(kModels_ZWLD_100)/sizeof(kModels_ZWLD_100[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZWLD-100", .vendor="Trust",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZWLD_100, .from_zigbee_count=sizeof(kFz_ZWLD_100)/sizeof(kFz_ZWLD_100[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::trust
