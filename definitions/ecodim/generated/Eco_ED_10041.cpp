// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecodim ED-10041 — CCT filament light (hand-rewritten, 2026-04-28q).
// Zigbee LED filament light dimmable E27, edison ST64, flame 2200K.
// z2m-source: ecodim.ts #ED-10041.
//
// fingerprint:
//   modelID "CCT Light", manufacturerName "ZigBee/CCT",   manufacturerID 4137
//   modelID "CCT Light", manufacturerName "Astuta/ZB-CCT", manufacturerID 4137
// extend: tuya.modernExtend.tuyaLight({colorTemp: {range: [153, 454]}}).
//
// manufacturer_names array disambiguates from ajax_online's
// ZB-CCT_Filament def (also "CCT Light", manuName "ZB/Ajax Online").
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
const FzConverter* const kFz_ED_10041[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_ED_10041[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_ED_10041[] = { "CCT Light" };
constexpr const char* kManus_ED_10041[]  = { "ZigBee/CCT", "Astuta/ZB-CCT" };

constexpr Expose kExposes_ED_10041[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ED_10041[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_ED_10041{
    .zigbee_models=kModels_ED_10041, .zigbee_models_count=sizeof(kModels_ED_10041)/sizeof(kModels_ED_10041[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ED_10041, .manufacturer_names_count=sizeof(kManus_ED_10041)/sizeof(kManus_ED_10041[0]),
    .model="ED-10041", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_ED_10041, .exposes_count=sizeof(kExposes_ED_10041)/sizeof(kExposes_ED_10041[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ED_10041, .from_zigbee_count=sizeof(kFz_ED_10041)/sizeof(kFz_ED_10041[0]),
    .to_zigbee=kTz_ED_10041, .to_zigbee_count=sizeof(kTz_ED_10041)/sizeof(kTz_ED_10041[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ED_10041, .bindings_count=sizeof(kBindings_ED_10041)/sizeof(kBindings_ED_10041[0]),
};

}  // namespace zhc::devices::ecodim
