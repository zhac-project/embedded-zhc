// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Terncy DL001 — generic on/off + level + color_temp.
// Two color-temperature intelligent downlight (range 156..476 mireds).
// The auto-generator collapsed `m.light({colorTemp: {range: …}})` to
// on/off+brightness only, dropping the CT axis. Hand-rewritten to add
// kFzColorTemperature / kTzColorTemp + the lightingColorCtrl binding.
// z2m-source: terncy.ts #DL001.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

const FzConverter* const kFz_DL001[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_DL001[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_DL001[] = { "DL001" };

constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_DL001{
    .zigbee_models=kModels_DL001, .zigbee_models_count=sizeof(kModels_DL001)/sizeof(kModels_DL001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DL001", .vendor="Terncy",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DL001, .from_zigbee_count=sizeof(kFz_DL001)/sizeof(kFz_DL001[0]),
    .to_zigbee=kTz_DL001, .to_zigbee_count=sizeof(kTz_DL001)/sizeof(kTz_DL001[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::terncy
