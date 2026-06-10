// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Envilar 1CH-HP-RELAY-7853 — hand-rewritten for parity.
// 1 channel high power box relay
// z2m-source: envilar.ts #1CH-HP-RELAY-7853.
//
// z2m bundle: m.onOff({powerOnBehavior:true}), m.identify(),
//   m.electricityMeter(). The electricityMeter modernExtend defaults to
//   cluster:"both" + electricalMeasurementType:"ac", so it decodes BOTH
//   seMetering 0x0702 (energy) AND haElectricalMeasurement 0x0B04
//   (power/voltage/current). The previous auto-port wired only
//   kFzMetering (0x0702) and exposed energy+power, dropping the entire
//   0x0B04 half (voltage + current). Restored kFzElectricalMeasurement,
//   the voltage/current exposes, and the 0x0B04 binding to match z2m.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::envilar {
namespace {
const FzConverter* const kFz_D1CH_HP_RELAY_7853[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_D1CH_HP_RELAY_7853[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_D1CH_HP_RELAY_7853[] = { "7853" };

}  // namespace


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


constexpr WhiteLabel kWhiteLabels_D1CH_HP_RELAY_7853[] = {
    {"Sunricher","SR-ZG9101SAC-HP-SWITCH-B"},
};
extern const PreparedDefinition kDef_D1CH_HP_RELAY_7853{
    .zigbee_models=kModels_D1CH_HP_RELAY_7853, .zigbee_models_count=sizeof(kModels_D1CH_HP_RELAY_7853)/sizeof(kModels_D1CH_HP_RELAY_7853[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1CH-HP-RELAY-7853", .vendor="Envilar",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_D1CH_HP_RELAY_7853, .white_labels_count=sizeof(kWhiteLabels_D1CH_HP_RELAY_7853)/sizeof(kWhiteLabels_D1CH_HP_RELAY_7853[0]),
    .from_zigbee=kFz_D1CH_HP_RELAY_7853, .from_zigbee_count=sizeof(kFz_D1CH_HP_RELAY_7853)/sizeof(kFz_D1CH_HP_RELAY_7853[0]),
    .to_zigbee=kTz_D1CH_HP_RELAY_7853, .to_zigbee_count=sizeof(kTz_D1CH_HP_RELAY_7853)/sizeof(kTz_D1CH_HP_RELAY_7853[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::envilar
