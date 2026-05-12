// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Livolo TI0001-illuminance — hand-ported custom cluster.
// Zigbee digital illuminance and sound sensor.
// z2m-source: livolo.ts #TI0001-illuminance.
#include "definitions/_generic/_shared.hpp"
#include "definitions/livolo/_shared.hpp"

namespace zhc::devices::livolo {
namespace {

const FzConverter* const kFz_TI0001_illuminance[] = {
    &::zhc::devices::livolo::kFzLivoloIlluminance,
};

constexpr const char* kModels_TI0001_illuminance[] = { "TI0001-illuminance" };

constexpr Expose kAutoExposes[] = {
    { "illuminance",    ExposeType::Numeric, Access::State, "lx",      nullptr, nullptr, 0 },
    { "noise_detected", ExposeType::Binary,  Access::State, nullptr,   nullptr, nullptr, 0 },
    { "noise_level",    ExposeType::Numeric, Access::State, nullptr,   nullptr, nullptr, 0 },
};

constexpr BindingSpec kAutoBindings[] = {
    { 1, 0x0001 },
};

}  // namespace

extern const PreparedDefinition kDef_TI0001_illuminance{
    .zigbee_models=kModels_TI0001_illuminance,
    .zigbee_models_count=sizeof(kModels_TI0001_illuminance)/sizeof(kModels_TI0001_illuminance[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TI0001-illuminance", .vendor="Livolo",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TI0001_illuminance,
    .from_zigbee_count=sizeof(kFz_TI0001_illuminance)/sizeof(kFz_TI0001_illuminance[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::livolo
