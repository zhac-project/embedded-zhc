// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Evn ZB24100VS — graduated from generated/ for a parity fix.
// Zigbee multicolor controller with power supply (RGB+CCT light).
// z2m-source: evn.ts #ZB24100VS.
//
// Parity fix (was on/off + brightness only): z2m extends this as
//   m.light({colorTemp: {range: [160, 450]}, color: {modes: ["xy","hs"]}})
// i.e. a full colour + colour-temperature light on lightingColorCtrl
// (cluster 0x0300). The auto-generated def dropped the entire 0x0300 axis
// — no colour, no colour-temperature read or write. Restored below:
//   * kFzColor (xy + hs attrs → hue/saturation/color_x/color_y/color_temp)
//   * kFzColorTemperature (colorTempMireds → color_temp)
//   * kTzColor + kTzColorTemp (the writeable colour/CT path)
//   * color_temp / hue / saturation / color_x / color_y exposes
//   * bind lightingColorCtrl (0x0300).
// colorTemp range 160..450 mireds carried on the color_temp expose.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::evn {
namespace {
const FzConverter* const kFz_ZB24100VS[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_ZB24100VS[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_ZB24100VS[] = { "ZB24100VS" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 160, 450, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl (colour + colour-temperature)
};

extern const PreparedDefinition kDef_ZB24100VS{
    .zigbee_models=kModels_ZB24100VS, .zigbee_models_count=sizeof(kModels_ZB24100VS)/sizeof(kModels_ZB24100VS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB24100VS", .vendor="Evn",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB24100VS, .from_zigbee_count=sizeof(kFz_ZB24100VS)/sizeof(kFz_ZB24100VS[0]),
    .to_zigbee=kTz_ZB24100VS, .to_zigbee_count=sizeof(kTz_ZB24100VS)/sizeof(kTz_ZB24100VS[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::evn
