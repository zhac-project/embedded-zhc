// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RGB Genie ZB-5121 — hand-rewritten 2026-04-28q.
// Micro remote and dimmer with single scene recall (battery, single-EP).
// z2m-source: rgb_genie.ts #ZB-5121.
//
// z2m bundle: fz.battery, fz.command_on, fz.command_off, fz.command_step,
//             fz.command_move, fz.command_stop, fz.command_recall.
//   exposes [battery, action(on, off, brightness_step_up,
//            brightness_step_down, brightness_move_up,
//            brightness_move_down, brightness_stop, recall_*)].
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. ZB-5121
//   is a remote (sender), not a sink. z2m's meta.battery.dontDividePercentage
//   stays a per-converter concern — kFzBattery currently divides; tracked gap.
#include "definitions/rgb_genie/_shared.hpp"

namespace zhc::devices::rgb_genie {
namespace {
constexpr const char* kModels_ZB_5121[] = { "RGBgenie ZB-5121" };
}  // namespace

constexpr Expose kExposes_ZB_5121[] = {
    { "action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0 },
};

extern const PreparedDefinition kDef_ZB_5121{
    .zigbee_models=kModels_ZB_5121,
    .zigbee_models_count=sizeof(kModels_ZB_5121)/sizeof(kModels_ZB_5121[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-5121", .vendor="RGB Genie",
    .meta=nullptr,
    .exposes=kExposes_ZB_5121,
    .exposes_count=sizeof(kExposes_ZB_5121)/sizeof(kExposes_ZB_5121[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzActionBatteryDim, .from_zigbee_count=kFzActionBatteryDimCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsActionBatteryDim, .bindings_count=kBindingsActionBatteryDimCount,
};

}  // namespace zhc::devices::rgb_genie
