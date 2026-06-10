// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Icasa ICZB-IW21D — dimmer + electricity meter.
// Zigbee AC dimmer. z2m: extend=[m.light({configureReporting:true}),
// m.electricityMeter()]. electricityMeter() defaults to cluster:"both"
// (lib/modernExtend.ts genericMeter) → it decodes BOTH seMetering (0x0702:
// energy 0x0000 / power 0x0400) AND haElectricalMeasurement (0x0B04: power
// 0x050B / voltage 0x0505 / current 0x0508), exposing power+voltage+current+
// energy (acFrequency/powerFactor/producedEnergy/tariffs all off by default).
// The auto-generated port wired only kFzMetering (the 0x0702 half) → it
// dropped voltage + current as dead channels. Fix: add kFzElectricalMeasurement
// (0x0B04) alongside, its binding, and the voltage/current exposes.
// z2m-source: icasa.ts #ICZB-IW21D + lib/modernExtend.ts electricityMeter().
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::icasa {
namespace {
const FzConverter* const kFz_ICZB_IW21D[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_ICZB_IW21D[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_ICZB_IW21D[] = { "ICZB-IW21D" };

}  // namespace


constexpr Expose kExposes_ICZB_IW21D[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ICZB_IW21D[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0B04},  // haElectricalMeasurement (power/voltage/current)
    {1, 0x0702},  // seMetering (energy/power)
};

extern const PreparedDefinition kDef_ICZB_IW21D{
    .zigbee_models=kModels_ICZB_IW21D, .zigbee_models_count=sizeof(kModels_ICZB_IW21D)/sizeof(kModels_ICZB_IW21D[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ICZB-IW21D", .vendor="Icasa",
    .meta=nullptr, .exposes=kExposes_ICZB_IW21D, .exposes_count=sizeof(kExposes_ICZB_IW21D)/sizeof(kExposes_ICZB_IW21D[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ICZB_IW21D, .from_zigbee_count=sizeof(kFz_ICZB_IW21D)/sizeof(kFz_ICZB_IW21D[0]),
    .to_zigbee=kTz_ICZB_IW21D, .to_zigbee_count=sizeof(kTz_ICZB_IW21D)/sizeof(kTz_ICZB_IW21D[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_ICZB_IW21D,.bindings_count=sizeof(kBindings_ICZB_IW21D)/sizeof(kBindings_ICZB_IW21D[0]),
};

}  // namespace zhc::devices::icasa
