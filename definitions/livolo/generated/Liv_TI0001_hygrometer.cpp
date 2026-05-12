// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Livolo TI0001-hygrometer — hand-ported custom cluster.
// Zigbee Digital Humidity and Temperature Sensor.
// z2m-source: livolo.ts #TI0001-hygrometer.
#include "definitions/_generic/_shared.hpp"
#include "definitions/livolo/_shared.hpp"

namespace zhc::devices::livolo {
namespace {

const FzConverter* const kFz_TI0001_hygrometer[] = {
    &::zhc::devices::livolo::kFzLivoloHygrometer,
};

constexpr const char* kModels_TI0001_hygrometer[] = { "TI0001-hygrometer" };

constexpr Expose kAutoExposes[] = {
    { "temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0 },
};

constexpr BindingSpec kAutoBindings[] = {
    { 1, 0x0001 },
};

}  // namespace

extern const PreparedDefinition kDef_TI0001_hygrometer{
    .zigbee_models=kModels_TI0001_hygrometer,
    .zigbee_models_count=sizeof(kModels_TI0001_hygrometer)/sizeof(kModels_TI0001_hygrometer[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TI0001-hygrometer", .vendor="Livolo",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TI0001_hygrometer,
    .from_zigbee_count=sizeof(kFz_TI0001_hygrometer)/sizeof(kFz_TI0001_hygrometer[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::livolo
