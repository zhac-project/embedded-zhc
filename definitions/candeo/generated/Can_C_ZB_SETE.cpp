// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-ZB-SETE.
// Temperature & humidity sensor
// z2m-source: candeo.ts #C-ZB-SETE.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_SETE[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity
};
constexpr const char* kModels_C_ZB_SETE[] = { "C-ZB-SETE" };

constexpr Expose kAutoExposes[] = {
    {"battery",           ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "mV",    nullptr, nullptr, 0},
    {"temperature",       ExposeType::Numeric, Access::State,    "C",     nullptr, nullptr, 0},
    {"humidity",          ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405}
};

}  // namespace

extern const PreparedDefinition kDef_C_ZB_SETE{
    .zigbee_models=kModels_C_ZB_SETE, .zigbee_models_count=sizeof(kModels_C_ZB_SETE)/sizeof(kModels_C_ZB_SETE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-SETE", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_SETE, .from_zigbee_count=sizeof(kFz_C_ZB_SETE)/sizeof(kFz_C_ZB_SETE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
