// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Schwaiger HAL800 — hand-rewritten for parity (full CT+color).
// LED candle bulb E14 470 lumen, dimmable, color, white 1800-6500K.
// z2m drives it via m.light({colorTemp:{range:[153,555]}, color:true}) —
// both the lightingColorCtrl 0x0300 colour-temperature axis AND the
// XY/HS colour axis. The auto-port wired only kFzOnOff + kFzBrightness,
// dropping the entire colour channel. Restored kFzColor +
// kFzColorTemperature / kTzColor + kTzColorTemp + the colour/CT exposes +
// the 0x0300 binding.
// z2m-source: schwaiger.ts #HAL800.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::schwaiger {
namespace {
const FzConverter* const kFz_HAL800[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_HAL800[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_HAL800[] = { "ZBT-RGBWLight-C4700114" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_HAL800{
    .zigbee_models=kModels_HAL800, .zigbee_models_count=sizeof(kModels_HAL800)/sizeof(kModels_HAL800[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HAL800", .vendor="Schwaiger",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HAL800, .from_zigbee_count=sizeof(kFz_HAL800)/sizeof(kFz_HAL800[0]),
    .to_zigbee=kTz_HAL800, .to_zigbee_count=sizeof(kTz_HAL800)/sizeof(kTz_HAL800[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::schwaiger
