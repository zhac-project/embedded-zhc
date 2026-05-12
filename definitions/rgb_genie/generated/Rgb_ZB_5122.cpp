// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RGB Genie ZB-5122 — hand-rewritten 2026-04-28q.
// Micro remote and color dimmer with single scene recall (battery, single-EP).
// z2m-source: rgb_genie.ts #ZB-5122.
//
// z2m bundle: fz.battery, fz.command_on, fz.command_off, fz.command_step,
//             fz.command_move, fz.command_stop, fz.command_recall,
//             fz.command_move_to_color, fz.command_move_to_color_temp,
//             fz.command_move_hue, fz.command_move_color_temperature.
//   exposes [battery, action(on, off, brightness_*, recall_*,
//            color_temperature_move_up/_down, hue_move, hue_stop)].
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. Corrected
//   to action+battery+color/CT actions. PARTIAL: command_move_hue,
//   command_move_to_color, command_move_color_temperature are not yet wired
//   in _generic — only step CT, move-to-CT, move-to-hue-and-sat fire today.
#include "definitions/rgb_genie/_shared.hpp"

namespace zhc::devices::rgb_genie {
namespace {
constexpr const char* kModels_ZB_5122[] = { "RGBgenie ZB-5122" };
}  // namespace

constexpr Expose kExposes_ZB_5122[] = {
    { "action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0 },
};

extern const PreparedDefinition kDef_ZB_5122{
    .zigbee_models=kModels_ZB_5122,
    .zigbee_models_count=sizeof(kModels_ZB_5122)/sizeof(kModels_ZB_5122[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-5122", .vendor="RGB Genie",
    .meta=nullptr,
    .exposes=kExposes_ZB_5122,
    .exposes_count=sizeof(kExposes_ZB_5122)/sizeof(kExposes_ZB_5122[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzActionBatteryRGB, .from_zigbee_count=kFzActionBatteryRGBCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsActionBatteryRGB, .bindings_count=kBindingsActionBatteryRGBCount,
};

}  // namespace zhc::devices::rgb_genie
