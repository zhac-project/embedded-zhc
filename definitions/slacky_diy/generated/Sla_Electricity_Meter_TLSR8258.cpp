// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SlackyDiy Electricity Meter TLSR8258 — auto-generated.
// Electricity Meter via optical port
// z2m-source: slacky_diy.ts #Electricity Meter TLSR8258.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {
const FzConverter* const kFz_Electricity_Meter_TLSR8258[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};

constexpr const char* kModels_Electricity_Meter_TLSR8258[] = { "ElectricityMeter_DIY" };

}  // namespace


// z2m wires m.electricityMeter() (clusters seMetering 0x0702 + haElectricalMeasurement 0x0B04) plus
// m.deviceTemperature() and a stack of custom 0xFXXX seMetering attributes (deviceModel/address/
// readPeriod/dateRelease/deviceName/devicePassword) plus an electricityDeviceModel enumLookup.
// The standard energy/power/voltage/current cover the metering surface; the custom attrs are not
// decodable here.
constexpr Expose kAutoExposes[] = {
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702}, {1, 0x0B04},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_Electricity_Meter_TLSR8258{
    .zigbee_models=kModels_Electricity_Meter_TLSR8258, .zigbee_models_count=sizeof(kModels_Electricity_Meter_TLSR8258)/sizeof(kModels_Electricity_Meter_TLSR8258[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Electricity Meter TLSR8258", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Electricity_Meter_TLSR8258, .from_zigbee_count=sizeof(kFz_Electricity_Meter_TLSR8258)/sizeof(kFz_Electricity_Meter_TLSR8258[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::slacky_diy
