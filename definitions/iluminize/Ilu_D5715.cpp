// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Iluminize 5715 — micro smart dimmer with energy metering.
//
// z2m: m.light(...) + m.electricityMeter(). m.electricityMeter() defaults to
// cluster:"both" ⇒ fromZigbee=[fz.electrical_measurement, fz.metering],
// exposing power+voltage+current (haElectricalMeasurement 0x0B04) AND energy
// (seMetering 0x0702). The auto-gen port wired only kFzMetering (0x0702) and
// exposed energy+power, DROPPING the entire haElectricalMeasurement half —
// current and voltage never decoded. Graduated to add kFzElectricalMeasurement
// + the current/voltage exposes + the 0x0B04 binding.
// NB: sunricher.extend.externalSwitchType()/minimumPWM() are manuSpecific
// sunricher config attributes with no generic converter — deferred (INFRA).
// z2m-source: iluminize.ts #5715 + lib/modernExtend.ts electricityMeter
//             (cluster:"both") + converters/fromZigbee.ts electrical_measurement.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {
const FzConverter* const kFz_D5715[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,                // seMetering 0x0702 → energy (+power)
    &::zhc::generic::kFzElectricalMeasurement,   // haElectricalMeasurement 0x0B04 → power/voltage/current
};
const TzConverter* const kTz_D5715[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_D5715[] = { "5715" };

constexpr Expose kExposes_D5715[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",     ExposeType::Numeric, Access::State,    "kWh", nullptr, nullptr, 0},
    {"power",      ExposeType::Numeric, Access::State,    "W",   nullptr, nullptr, 0},
    {"voltage",    ExposeType::Numeric, Access::State,    "V",   nullptr, nullptr, 0},
    {"current",    ExposeType::Numeric, Access::State,    "A",   nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D5715[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0702},   // seMetering
    {1, 0x0B04},   // haElectricalMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_D5715{
    .zigbee_models=kModels_D5715, .zigbee_models_count=sizeof(kModels_D5715)/sizeof(kModels_D5715[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="5715", .vendor="Iluminize",
    .meta=nullptr, .exposes=kExposes_D5715, .exposes_count=sizeof(kExposes_D5715)/sizeof(kExposes_D5715[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D5715, .from_zigbee_count=sizeof(kFz_D5715)/sizeof(kFz_D5715[0]),
    .to_zigbee=kTz_D5715, .to_zigbee_count=sizeof(kTz_D5715)/sizeof(kTz_D5715[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D5715, .bindings_count=sizeof(kBindings_D5715)/sizeof(kBindings_D5715[0]),
};

}  // namespace zhc::devices::iluminize
