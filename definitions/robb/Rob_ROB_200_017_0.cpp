// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Robb ROB_200-017-0 — graduated from generated/ to fix a real parity gap.
// Zigbee smart plug (with internal temperature sensor).
// z2m-source: robb.ts #ROB_200-017-0 — fz.electrical_measurement, fz.on_off,
//   fz.metering, fz.temperature; exposes power/current/voltage/switch/energy/
//   temperature; configure binds msTemperatureMeasurement + reporting.temperature.
//
// PARITY FIX (missing temperature): the auto-generated port lowered on_off +
// metering + electrical_measurement but dropped fz.temperature, so the plug's
// built-in temperature reading (msTemperatureMeasurement, /100 °C) never
// surfaced. Add kFzTemperature + the `temperature` expose + the 0x0402 bind.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_017_0[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzTemperature,
};
const TzConverter* const kTz_ROB_200_017_0[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_ROB_200_017_0[] = { "ROB_200-017-0", "HK-PLUG-A" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",       ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",      ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power",       ExposeType::Numeric, Access::State, "W",   nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "V",   nullptr, nullptr, 0},
    {"current",     ExposeType::Numeric, Access::State, "A",   nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",   nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0702},  // seMetering
    {1, 0x0B04},  // haElectricalMeasurement
    {1, 0x0402},  // msTemperatureMeasurement
};

extern const PreparedDefinition kDef_ROB_200_017_0{
    .zigbee_models=kModels_ROB_200_017_0, .zigbee_models_count=sizeof(kModels_ROB_200_017_0)/sizeof(kModels_ROB_200_017_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-017-0", .vendor="Robb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ROB_200_017_0, .from_zigbee_count=sizeof(kFz_ROB_200_017_0)/sizeof(kFz_ROB_200_017_0[0]),
    .to_zigbee=kTz_ROB_200_017_0, .to_zigbee_count=sizeof(kTz_ROB_200_017_0)/sizeof(kTz_ROB_200_017_0[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::robb
