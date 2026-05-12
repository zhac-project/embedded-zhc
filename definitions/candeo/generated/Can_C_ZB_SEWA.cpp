// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-ZB-SEWA — water leak via IAS Zone alarm_1.
// Water sensor
// z2m-source: candeo.ts #C-ZB-SEWA.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_SEWA[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm
};
constexpr const char* kModels_C_ZB_SEWA[] = { "C-ZB-SEWA" };

constexpr Expose kAutoExposes[] = {
    {"battery",           ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "mV",    nullptr, nullptr, 0},
    {"water_leak",        ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"battery_low",       ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"tamper",            ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500}
};

}  // namespace

extern const PreparedDefinition kDef_C_ZB_SEWA{
    .zigbee_models=kModels_C_ZB_SEWA, .zigbee_models_count=sizeof(kModels_C_ZB_SEWA)/sizeof(kModels_C_ZB_SEWA[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-SEWA", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_SEWA, .from_zigbee_count=sizeof(kFz_C_ZB_SEWA)/sizeof(kFz_C_ZB_SEWA[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
