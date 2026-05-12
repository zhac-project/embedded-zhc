// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled Z01-A19NAE26 — hand-rewritten 2026-04-28.
// Element plus (A19)
// z2m-source: sengled.ts #Z01-A19NAE26.
// Mapping: sengledLight({colorTemp: {range: [154, 500]}}) + electricityMeter(metering)
//          -> CTLight bundle + per-port FZ/binding for seMetering.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {

const ::zhc::FzConverter* const kFz_Z01_A19NAE26[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzMetering,
};

constexpr const char* kModels_Z01_A19NAE26[] = { "Z01-A19NAE26" };

constexpr Expose kExposes_Z01_A19NAE26[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "energy",     ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "kWh",   nullptr, nullptr, 0 },
    { "power",      ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "W",     nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_Z01_A19NAE26[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 }, { 1, 0x0702 },
};

}  // namespace

extern const PreparedDefinition kDef_Z01_A19NAE26{
    .zigbee_models=kModels_Z01_A19NAE26, .zigbee_models_count=sizeof(kModels_Z01_A19NAE26)/sizeof(kModels_Z01_A19NAE26[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Z01-A19NAE26", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposes_Z01_A19NAE26, .exposes_count=sizeof(kExposes_Z01_A19NAE26)/sizeof(kExposes_Z01_A19NAE26[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Z01_A19NAE26, .from_zigbee_count=sizeof(kFz_Z01_A19NAE26)/sizeof(kFz_Z01_A19NAE26[0]),
    .to_zigbee=kTzSengledCTLight, .to_zigbee_count=kTzSengledCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_Z01_A19NAE26, .bindings_count=sizeof(kBindings_Z01_A19NAE26)/sizeof(kBindings_Z01_A19NAE26[0]),
};

}  // namespace zhc::devices::sengled
