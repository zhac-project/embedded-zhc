// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AjaxOnline ZB-CCT_Filament — hand-rewritten for parity (CCT axis).
// Zigbee LED filament light dimmable E27, edison ST64, flame 2200K
// z2m-source: ajax_online.ts #ZB-CCT_Filament.
//
// z2m bundle: m.light({colorTemp: {range: [153, 454]}}) (no color). The
//   colorTemp option adds the lightingColorCtrl (0x0300) colour-temperature
//   channel + a color_temp expose. The auto-port wired only kFzOnOff +
//   kFzBrightness and exposed state + brightness, dropping the entire CCT
//   channel. Restored kFzColorTemperature / kTzColorTemp, the color_temp
//   expose, and the 0x0300 binding.
//
// z2m matches this via a full fingerprint (modelID "CCT Light",
//   manufacturerName "ZB/Ajax Online", manufacturerID 4137). The auto-port
//   gated on the bare model only, which "CCT Light" shares with other
//   vendors; restored the manufacturer_name so the gate matches z2m.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ajax_online {
namespace {
const FzConverter* const kFz_ZB_CCT_Filament[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_ZB_CCT_Filament[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_ZB_CCT_Filament[] = { "CCT Light" };
constexpr const char* kManus_ZB_CCT_Filament[]  = { "ZB/Ajax Online" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_ZB_CCT_Filament{
    .zigbee_models=kModels_ZB_CCT_Filament, .zigbee_models_count=sizeof(kModels_ZB_CCT_Filament)/sizeof(kModels_ZB_CCT_Filament[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZB_CCT_Filament, .manufacturer_names_count=sizeof(kManus_ZB_CCT_Filament)/sizeof(kManus_ZB_CCT_Filament[0]),
    .model="ZB-CCT_Filament", .vendor="AjaxOnline",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_CCT_Filament, .from_zigbee_count=sizeof(kFz_ZB_CCT_Filament)/sizeof(kFz_ZB_CCT_Filament[0]),
    .to_zigbee=kTz_ZB_CCT_Filament, .to_zigbee_count=sizeof(kTz_ZB_CCT_Filament)/sizeof(kTz_ZB_CCT_Filament[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ajax_online
