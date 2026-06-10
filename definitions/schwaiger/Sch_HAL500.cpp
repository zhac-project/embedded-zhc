// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Schwaiger HAL500 — hand-rewritten for parity (CT axis).
// LED bulb GU10 350 lumen, dimmable, color, white 2700-6500K.
// z2m drives it via m.light({colorTemp:{range:[153,370]}}) — the
// lightingColorCtrl 0x0300 colour-temperature channel + a color_temp
// expose. The auto-port wired only kFzOnOff + kFzBrightness, dropping the
// entire CCT channel. Restored kFzColorTemperature / kTzColorTemp + the
// color_temp expose + the 0x0300 binding.
// z2m-source: schwaiger.ts #HAL500.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::schwaiger {
namespace {
const FzConverter* const kFz_HAL500[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_HAL500[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_HAL500[] = { "ZBT-CCTLight-GU100904" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_HAL500{
    .zigbee_models=kModels_HAL500, .zigbee_models_count=sizeof(kModels_HAL500)/sizeof(kModels_HAL500[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HAL500", .vendor="Schwaiger",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HAL500, .from_zigbee_count=sizeof(kFz_HAL500)/sizeof(kFz_HAL500[0]),
    .to_zigbee=kTz_HAL500, .to_zigbee_count=sizeof(kTz_HAL500)/sizeof(kTz_HAL500[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::schwaiger
