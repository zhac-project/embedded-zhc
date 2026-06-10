// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lifecontrol MCLH-03 — hand-rewritten (vendor parity sweep).
// Smart socket (on/off + full power metering).
// z2m-source: lifecontrol.ts #MCLH-03.
//
// z2m: m.onOff + m.electricityMeter({configureReporting: false}) +
// electricityMeterPoll(). m.electricityMeter() defaults to cluster
// "both" → seMetering (0x0702) energy AND haElectricalMeasurement
// (0x0B04) power/voltage/current. The custom poll reads exactly
// haElectricalMeasurement.{rmsVoltage,rmsCurrent,activePower} and
// seMetering.currentSummDelivered. The auto-generated port wired only
// kFzMetering (0x0702) → energy/power and dropped the entire 0x0B04
// half (voltage/current dead). Wire kFzElectricalMeasurement alongside
// and expose voltage/current + the 0x0B04 bind.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lifecontrol {
namespace {
const FzConverter* const kFz_MCLH_03[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,             // seMetering 0x0702 → energy
    &::zhc::generic::kFzElectricalMeasurement,// haElectricalMeasurement 0x0B04 → power/voltage/current
};
const TzConverter* const kTz_MCLH_03[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_MCLH_03[] = { "RICI01" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",  ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power",   ExposeType::Numeric, Access::State, "W",   nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V",   nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A",   nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0702},  // seMetering
    {1, 0x0B04},  // haElectricalMeasurement
};

extern const PreparedDefinition kDef_MCLH_03{
    .zigbee_models=kModels_MCLH_03, .zigbee_models_count=sizeof(kModels_MCLH_03)/sizeof(kModels_MCLH_03[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MCLH-03", .vendor="Lifecontrol",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MCLH_03, .from_zigbee_count=sizeof(kFz_MCLH_03)/sizeof(kFz_MCLH_03[0]),
    .to_zigbee=kTz_MCLH_03, .to_zigbee_count=sizeof(kTz_MCLH_03)/sizeof(kTz_MCLH_03[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lifecontrol
