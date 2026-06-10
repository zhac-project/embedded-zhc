// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RGB Genie ZB-5028 — graduated from generated/ 2026-06-10.
// RGB remote with 4 endpoints and 3 scene recalls (battery, multi-EP).
// z2m-source: rgb_genie.ts #ZB-5028.
//
// z2m bundle: fz.battery, fz.command_on, fz.command_off, fz.command_step,
//             fz.command_move, fz.command_stop, fz.command_recall,
//             fz.command_move_hue, fz.command_move_to_color,
//             fz.command_move_to_color_temp.
//   exposes [battery, action(on, off, brightness_*, recall_1..3,
//            hue_move, color_temperature_move, color_move, hue_stop)].
//   meta: {multiEndpoint: true, battery: {dontDividePercentage: true}}.
// FIX (2026-06-10): z2m's `meta.multiEndpoint` with no `endpoint()` map
//   postfixes every action with the raw endpoint ID (utils.ts
//   postfixWithEndpointName → `value_<msg.endpoint.ID>`), so EP1..EP4
//   emit distinct actions (e.g. recall_1_4). The earlier port declared
//   `endpoint_map` but forgot `.endpoint_action_suffix = true`, so the
//   dispatch left `action` GLOBAL (kAlwaysGlobalKeys) and all four
//   buttons collided on a single bare key — sibling ZB-3008 (also
//   multiEndpoint) had the flag, this one was missed. Flag added below.
//   command_move_hue / command_move_to_color ARE wired in _generic now
//   (kFzActionBatteryRGB pulls them in), so hue_move / color_move fire.
#include "definitions/rgb_genie/_shared.hpp"

namespace zhc::devices::rgb_genie {
namespace {
constexpr const char* kModels_ZB_5028[] = { "RGBgenie ZB-5028" };

constexpr ::zhc::EndpointLabel kEndpoints_ZB_5028[] = {
    { "ep1", 1 },
    { "ep2", 2 },
    { "ep3", 3 },
    { "ep4", 4 },
};
}  // namespace

constexpr Expose kExposes_ZB_5028[] = {
    { "action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0 },
};

extern const PreparedDefinition kDef_ZB_5028{
    .zigbee_models=kModels_ZB_5028,
    .zigbee_models_count=sizeof(kModels_ZB_5028)/sizeof(kModels_ZB_5028[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-5028", .vendor="RGB Genie",
    .meta=nullptr,
    .exposes=kExposes_ZB_5028,
    .exposes_count=sizeof(kExposes_ZB_5028)/sizeof(kExposes_ZB_5028[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzActionBatteryRGB, .from_zigbee_count=kFzActionBatteryRGBCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsActionBatteryRGB, .bindings_count=kBindingsActionBatteryRGBCount,
    .endpoint_map=kEndpoints_ZB_5028,
    .endpoint_map_count=sizeof(kEndpoints_ZB_5028)/sizeof(kEndpoints_ZB_5028[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::rgb_genie
