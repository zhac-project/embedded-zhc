// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HEIMAN HS2VTD 4-in-1 door sensor (contact + vibration + T/H) (z2m v26.95.0 parity).
// z2m-source: heiman.ts #HS2VTD (added v26.94.0)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS2VTD[] = {
    &::zhc::generic::kFzIasContactAlarm,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_HS2VTD[] = { "HS2VTD" };

constexpr Expose kExposes_HS2VTD[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, "Contact state", nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, "Battery low", nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\u00b0C", "Measured temperature", nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", "Measured relative humidity", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", "Battery percentage", nullptr, 0},
};

constexpr BindingSpec kBindings_HS2VTD[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0402},
    {1, 0x0405},
};
}  // namespace

extern const PreparedDefinition kDef_HS2VTD{
    .zigbee_models=kModels_HS2VTD, .zigbee_models_count=sizeof(kModels_HS2VTD)/sizeof(kModels_HS2VTD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS2VTD", .vendor="HEIMAN",
    .meta=nullptr, .exposes=kExposes_HS2VTD, .exposes_count=sizeof(kExposes_HS2VTD)/sizeof(kExposes_HS2VTD[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS2VTD, .from_zigbee_count=sizeof(kFz_HS2VTD)/sizeof(kFz_HS2VTD[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HS2VTD, .bindings_count=sizeof(kBindings_HS2VTD)/sizeof(kBindings_HS2VTD[0]),
};

}  // namespace zhc::devices::heiman
