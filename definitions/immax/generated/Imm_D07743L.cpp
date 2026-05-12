// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Immax 07743L — hand-rewritten (TS0505B RGB+CCT mains-powered bulb).
// Neo Smart LED E27 11W RGB + CCT, color, dimmable, Zigbee 3.0
// z2m-source: immax.ts #07743L.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::immax {
namespace {

constexpr const char* kModels_D07743L[] = { "TS0505B" };
constexpr const char* kManus_D07743L[] = { "_TZ3210_pwauw3g2" };

const FzConverter* const kFz_D07743L[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_D07743L[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr Expose kAutoExposes[] = {
    { "state",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_temp", ExposeType::Numeric, ::zhc::Access::StateSet, "mired",  nullptr, nullptr, 0 },
    { "color",      ExposeType::String,  ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
};
constexpr BindingSpec kAutoBindings[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
}  // namespace
extern const PreparedDefinition kDef_D07743L{
    .zigbee_models=kModels_D07743L, .zigbee_models_count=sizeof(kModels_D07743L)/sizeof(kModels_D07743L[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_D07743L, .manufacturer_names_count=sizeof(kManus_D07743L)/sizeof(kManus_D07743L[0]),
    .model="07743L", .vendor="Immax",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D07743L, .from_zigbee_count=sizeof(kFz_D07743L)/sizeof(kFz_D07743L[0]),
    .to_zigbee=kTz_D07743L, .to_zigbee_count=sizeof(kTz_D07743L)/sizeof(kTz_D07743L[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kAutoBindings,
    .bindings_count           = sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::immax
