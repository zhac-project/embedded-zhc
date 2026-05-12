// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SlackyDiy SLACKY_DIY_CO2_SENSOR_R01 — auto-generated.
// Tuya CO2 sensor with custom Firmware
// z2m-source: slacky_diy.ts #SLACKY_DIY_CO2_SENSOR_R01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {


constexpr const char* kModels_SLACKY_DIY_CO2_SENSOR_R01[] = { "Tuya_CO2Sensor_r01" };

}  // namespace


namespace {
const FzConverter* const kFz_min_SLACKY_DIY_CO2_SENSOR_R01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SLACKY_DIY_CO2_SENSOR_R01[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SLACKY_DIY_CO2_SENSOR_R01[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_SLACKY_DIY_CO2_SENSOR_R01{
    .zigbee_models=kModels_SLACKY_DIY_CO2_SENSOR_R01, .zigbee_models_count=sizeof(kModels_SLACKY_DIY_CO2_SENSOR_R01)/sizeof(kModels_SLACKY_DIY_CO2_SENSOR_R01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SLACKY_DIY_CO2_SENSOR_R01", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kExp_min_SLACKY_DIY_CO2_SENSOR_R01, .exposes_count=sizeof(kExp_min_SLACKY_DIY_CO2_SENSOR_R01)/sizeof(kExp_min_SLACKY_DIY_CO2_SENSOR_R01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SLACKY_DIY_CO2_SENSOR_R01, .from_zigbee_count=sizeof(kFz_min_SLACKY_DIY_CO2_SENSOR_R01)/sizeof(kFz_min_SLACKY_DIY_CO2_SENSOR_R01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_SLACKY_DIY_CO2_SENSOR_R01,
    .bindings_count           = sizeof(kBind_min_SLACKY_DIY_CO2_SENSOR_R01)/sizeof(kBind_min_SLACKY_DIY_CO2_SENSOR_R01[0]),
};

}  // namespace zhc::devices::slacky_diy
