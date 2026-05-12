// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SlackyDiy TS0201-z-SlD — auto-generated.
// Tuya temperature and humidity sensor with custom Firmware
// z2m-source: slacky_diy.ts #TS0201-z-SlD.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {
const FzConverter* const kFz_TS0201_z_SlD[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_TS0201_z_SlD[] = { "TS0201-z-SlD", "TS0201-z15-SlD", "TS0201-z21-SlD", "TS0201-z22-SlD", "TS0201-z23-SlD", "TS0201-z24-SlD" };

}  // namespace


// z2m TS0201-z-SlD wires m.temperature() + m.humidity() + m.battery({voltage:true}) plus four
// numeric extends against custom 0xFXXX attrs (temperature_offset/on_off/low/high) and the
// matching humidity quartet, m.deviceEndpoints, and an enum repeat_command lookup. None of the
// custom 0xFXXX attrs decode in this runtime; the standard temp/hum/battery do.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001}, {1, 0x0402}, {1, 0x0405},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_TS0201_z_SlD{
    .zigbee_models=kModels_TS0201_z_SlD, .zigbee_models_count=sizeof(kModels_TS0201_z_SlD)/sizeof(kModels_TS0201_z_SlD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TS0201-z-SlD", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TS0201_z_SlD, .from_zigbee_count=sizeof(kFz_TS0201_z_SlD)/sizeof(kFz_TS0201_z_SlD[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::slacky_diy
