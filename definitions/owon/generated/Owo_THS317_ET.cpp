// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon THS317-ET — auto-generated.
// Temperature sensor
// z2m-source: owon.ts #THS317-ET.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz_THS317_ET[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_THS317_ET[] = { "THS317-ET" };

}  // namespace


// z2m: e.battery(), e.battery_voltage(), e.temperature(); reporting on ep3 (fallback ep1).
constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {3, 0x0001},  // genPowerCfg
    {3, 0x0402},  // msTemperatureMeasurement
};

extern const PreparedDefinition kDef_THS317_ET{
    .zigbee_models=kModels_THS317_ET, .zigbee_models_count=sizeof(kModels_THS317_ET)/sizeof(kModels_THS317_ET[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="THS317-ET", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_THS317_ET, .from_zigbee_count=sizeof(kFz_THS317_ET)/sizeof(kFz_THS317_ET[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
