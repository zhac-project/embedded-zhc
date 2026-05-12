// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yandex YNDX_00529 — auto-generated.
// Temperature and humidity and pressure sensor
// z2m-source: yandex.ts #YNDX_00529.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yandex {
namespace {
const FzConverter* const kFz_YNDX_00529[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_YNDX_00529[] = { "YNDX-00529" };

}  // namespace


// --- hand-authored exposes / bindings (parity sweep 2026-04-28) ---
constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C",   nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"pressure",    ExposeType::Numeric, Access::State, "hPa", nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",  nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0403},  // msPressureMeasurement
    {1, 0x0001},  // genPowerCfg
};
// --- end ---

extern const PreparedDefinition kDef_YNDX_00529{
    .zigbee_models=kModels_YNDX_00529, .zigbee_models_count=sizeof(kModels_YNDX_00529)/sizeof(kModels_YNDX_00529[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX_00529", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00529, .from_zigbee_count=sizeof(kFz_YNDX_00529)/sizeof(kFz_YNDX_00529[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yandex
