// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E11-G13 — hand-rewritten 2026-04-28.
// Element classic (A19)
// z2m-source: sengled.ts #E11-G13.
// Mapping: forcePowerSource + sengledLight() + electricityMeter(metering)
//          -> Light bundle + per-port FZ/binding extension for the seMetering cluster.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {

const ::zhc::FzConverter* const kFz_E11_G13[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
};

constexpr const char* kModels_E11_G13[] = { "E11-G13" };

constexpr Expose kExposes_E11_G13[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "energy",     ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "kWh",   nullptr, nullptr, 0 },
    { "power",      ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "W",     nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_E11_G13[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0702 },
};

}  // namespace

extern const PreparedDefinition kDef_E11_G13{
    .zigbee_models=kModels_E11_G13, .zigbee_models_count=sizeof(kModels_E11_G13)/sizeof(kModels_E11_G13[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E11-G13", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposes_E11_G13, .exposes_count=sizeof(kExposes_E11_G13)/sizeof(kExposes_E11_G13[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E11_G13, .from_zigbee_count=sizeof(kFz_E11_G13)/sizeof(kFz_E11_G13[0]),
    .to_zigbee=kTzSengledLight, .to_zigbee_count=kTzSengledLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E11_G13, .bindings_count=sizeof(kBindings_E11_G13)/sizeof(kBindings_E11_G13[0]),
};

}  // namespace zhc::devices::sengled
