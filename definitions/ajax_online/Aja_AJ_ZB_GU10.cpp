// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AjaxOnline AJ_ZB_GU10 — hand-rewritten for parity (full colour).
// Smart Zigbee pro GU10 spotlight bulb
// z2m-source: ajax_online.ts #AJ_ZB_GU10.
//
// z2m bundle: m.light({colorTemp: {range: [158, 495]}, color: true}). The
//   color + colorTemp options add the lightingColorCtrl (0x0300) XY/HS
//   colour axis AND the colour-temperature axis. The auto-port wired only
//   kFzOnOff + kFzBrightness and exposed state + brightness, dropping the
//   entire colour channel. Restored kFzColor / kFzColorTemperature +
//   kTzColor / kTzColorTemp, the colour + color_temp exposes, and the
//   0x0300 binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ajax_online {
namespace {
const FzConverter* const kFz_AJ_ZB_GU10[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_AJ_ZB_GU10[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_AJ_ZB_GU10[] = { "AJ_ZB30_GU10", "AJ_ZB120_GU10" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
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

extern const PreparedDefinition kDef_AJ_ZB_GU10{
    .zigbee_models=kModels_AJ_ZB_GU10, .zigbee_models_count=sizeof(kModels_AJ_ZB_GU10)/sizeof(kModels_AJ_ZB_GU10[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AJ_ZB_GU10", .vendor="AjaxOnline",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AJ_ZB_GU10, .from_zigbee_count=sizeof(kFz_AJ_ZB_GU10)/sizeof(kFz_AJ_ZB_GU10[0]),
    .to_zigbee=kTz_AJ_ZB_GU10, .to_zigbee_count=sizeof(kTz_AJ_ZB_GU10)/sizeof(kTz_AJ_ZB_GU10[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ajax_online
