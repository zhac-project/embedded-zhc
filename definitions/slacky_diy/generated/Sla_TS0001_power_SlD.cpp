// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SlackyDiy TS0001_power-SlD — auto-generated.
// Switch with power monitoring
// z2m-source: slacky_diy.ts #TS0001_power-SlD.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {
const FzConverter* const kFz_TS0001_power_SlD[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_TS0001_power_SlD[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_TS0001_power_SlD[] = { "TS0001_power-SlD" };

}  // namespace


// z2m wires m.onOff({powerOnBehavior}) + m.electricityMeter() + custom 0xFXXX seMetering attrs
// (matching the TS011F-SlD plug). Standard kFzMetering+kFzElectricalMeasurement cover the
// energy/power/voltage/current pipeline.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_TS0001_power_SlD{
    .zigbee_models=kModels_TS0001_power_SlD, .zigbee_models_count=sizeof(kModels_TS0001_power_SlD)/sizeof(kModels_TS0001_power_SlD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TS0001_power-SlD", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TS0001_power_SlD, .from_zigbee_count=sizeof(kFz_TS0001_power_SlD)/sizeof(kFz_TS0001_power_SlD[0]),
    .to_zigbee=kTz_TS0001_power_SlD, .to_zigbee_count=sizeof(kTz_TS0001_power_SlD)/sizeof(kTz_TS0001_power_SlD[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::slacky_diy
