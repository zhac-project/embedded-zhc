// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RGB Genie ZB-3008 — hand-rewritten 2026-04-28q.
// 3 scene remote and dimmer (mains, multi-EP).
// z2m-source: rgb_genie.ts #ZB-3008.
//
// z2m bundle: fz.command_recall, fz.command_move_hue, fz.command_move,
//             fz.command_stop, fz.command_on, fz.command_off,
//             fz.command_move_to_color_temp, fz.command_move_to_color,
//             fz.command_move_color_temperature.
//   exposes [action(...)], meta: {multiEndpoint: true}.
// NOTE: previous port wired kFzOnOff and exposed `state`. ZB-3008 is a
//   3-button mains-powered scene remote (sender). multiEndpoint maps each
//   button to a separate ZCL endpoint (1..3). PARTIAL: command_move_hue,
//   command_move_to_color, command_move_color_temperature are not yet
//   wired in _generic.
#include "definitions/rgb_genie/_shared.hpp"

namespace zhc::devices::rgb_genie {
namespace {
constexpr const char* kModels_ZB_3008[] = { "RGBgenie ZB-3008" };

constexpr ::zhc::EndpointLabel kEndpoints_ZB_3008[] = {
    { "ep1", 1 },
    { "ep2", 2 },
    { "ep3", 3 },
};
}  // namespace

constexpr Expose kExposes_ZB_3008[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0 },
};

extern const PreparedDefinition kDef_ZB_3008{
    .zigbee_models=kModels_ZB_3008,
    .zigbee_models_count=sizeof(kModels_ZB_3008)/sizeof(kModels_ZB_3008[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-3008", .vendor="RGB Genie",
    .meta=nullptr,
    .exposes=kExposes_ZB_3008,
    .exposes_count=sizeof(kExposes_ZB_3008)/sizeof(kExposes_ZB_3008[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzActionRGBNoBattery, .from_zigbee_count=kFzActionRGBNoBatteryCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsActionRGBNoBattery, .bindings_count=kBindingsActionRGBNoBatteryCount,
    .endpoint_map=kEndpoints_ZB_3008,
    .endpoint_map_count=sizeof(kEndpoints_ZB_3008)/sizeof(kEndpoints_ZB_3008[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::rgb_genie
