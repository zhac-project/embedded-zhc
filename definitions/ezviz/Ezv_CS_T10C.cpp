// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: EZVIZ CS-T10C — water leak sensor.
// New upstream in z2m v26.100.0.
// z2m-source: ezviz.ts #CS-T10C.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ezviz {
namespace {
const FzConverter* const kFz_CS_T10C[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_CS_T10C[] = { "CS-T10C-A0-BG" };

constexpr Expose kExposes_CS_T10C[] = {
    {"water_leak",  ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_CS_T10C[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0500},   // ssIasZone
};
}  // namespace

extern const PreparedDefinition kDef_CS_T10C{
    .zigbee_models=kModels_CS_T10C, .zigbee_models_count=sizeof(kModels_CS_T10C)/sizeof(kModels_CS_T10C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CS-T10C", .vendor="EZVIZ",
    .meta=nullptr,
    .exposes=kExposes_CS_T10C, .exposes_count=sizeof(kExposes_CS_T10C)/sizeof(kExposes_CS_T10C[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CS_T10C, .from_zigbee_count=sizeof(kFz_CS_T10C)/sizeof(kFz_CS_T10C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_CS_T10C, .bindings_count=sizeof(kBindings_CS_T10C)/sizeof(kBindings_CS_T10C[0]),
};

}  // namespace zhc::devices::ezviz
