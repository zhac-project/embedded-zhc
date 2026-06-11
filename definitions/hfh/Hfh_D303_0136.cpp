// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Hfh 303-0136 — hand-rewritten for parity.
// LED controller
// z2m-source: hfh.ts #303-0136.
//
// z2m bundle: m.light({colorTemp: {range: [155, 495]}}). The colorTemp
//   option adds the lightingColorCtrl (0x0300) color-temperature channel
//   and a color_temp expose (mireds, 155-495). The previous auto-port
//   wired only kFzOnOff + kFzBrightness and exposed state + brightness,
//   dropping the entire CCT channel. Restored kFzColorTemperature /
//   kTzColorTemp, the color_temp expose, and the 0x0300 binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hfh {
namespace {
const FzConverter* const kFz_D303_0136[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_D303_0136[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_D303_0136[] = { "On-Air Combi CTW,303-0136" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 155, 495, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_D303_0136{
    .zigbee_models=kModels_D303_0136, .zigbee_models_count=sizeof(kModels_D303_0136)/sizeof(kModels_D303_0136[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="303-0136", .vendor="Hfh",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D303_0136, .from_zigbee_count=sizeof(kFz_D303_0136)/sizeof(kFz_D303_0136[0]),
    .to_zigbee=kTz_D303_0136, .to_zigbee_count=sizeof(kTz_D303_0136)/sizeof(kTz_D303_0136[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hfh
