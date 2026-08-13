// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HEIMAN S1-TL smoke alarm (z2m v26.95.0 parity).
// z2m-source: heiman.ts #S1-TL (added v26.94.0)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_S1_TL[] = {
    &::zhc::generic::kFzIasSmokeAlarm,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_S1_TL[] = { "S1-TL" };

constexpr Expose kExposes_S1_TL[] = {
    {"smoke", ExposeType::Binary, Access::State, nullptr, "Smoke detected", nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, "Battery low", nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\u00b0C", "Measured temperature", nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", "Measured relative humidity", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", "Battery percentage", nullptr, 0},
};

constexpr BindingSpec kBindings_S1_TL[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0402},
    {1, 0x0405},
};
}  // namespace

extern const PreparedDefinition kDef_S1_TL{
    .zigbee_models=kModels_S1_TL, .zigbee_models_count=sizeof(kModels_S1_TL)/sizeof(kModels_S1_TL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S1-TL", .vendor="HEIMAN",
    .meta=nullptr, .exposes=kExposes_S1_TL, .exposes_count=sizeof(kExposes_S1_TL)/sizeof(kExposes_S1_TL[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S1_TL, .from_zigbee_count=sizeof(kFz_S1_TL)/sizeof(kFz_S1_TL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_S1_TL, .bindings_count=sizeof(kBindings_S1_TL)/sizeof(kBindings_S1_TL[0]),
};

}  // namespace zhc::devices::heiman
