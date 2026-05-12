// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E21-N14A — hand-rewritten 2026-04-28.
// Smart light bulb, dimmable 5000K, E26/A19
// z2m-source: sengled.ts #E21-N14A.
// Mapping: sengledLight() + electricityMeter(metering)
//          -> Light bundle + per-port FZ/binding for seMetering.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {

const ::zhc::FzConverter* const kFz_E21_N14A[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
};

constexpr const char* kModels_E21_N14A[] = { "E21-N14A" };

constexpr Expose kExposes_E21_N14A[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "energy",     ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "kWh",   nullptr, nullptr, 0 },
    { "power",      ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "W",     nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_E21_N14A[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0702 },
};

}  // namespace

extern const PreparedDefinition kDef_E21_N14A{
    .zigbee_models=kModels_E21_N14A, .zigbee_models_count=sizeof(kModels_E21_N14A)/sizeof(kModels_E21_N14A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E21-N14A", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposes_E21_N14A, .exposes_count=sizeof(kExposes_E21_N14A)/sizeof(kExposes_E21_N14A[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E21_N14A, .from_zigbee_count=sizeof(kFz_E21_N14A)/sizeof(kFz_E21_N14A[0]),
    .to_zigbee=kTzSengledLight, .to_zigbee_count=kTzSengledLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E21_N14A, .bindings_count=sizeof(kBindings_E21_N14A)/sizeof(kBindings_E21_N14A[0]),
};

}  // namespace zhc::devices::sengled
