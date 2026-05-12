// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RGB Genie ZB-5004 — hand-rewritten 2026-04-28q.
// Zigbee 3.0 remote control (battery, single-EP).
// z2m-source: rgb_genie.ts #ZB-5004.
//
// z2m bundle: fz.command_recall, fz.command_on, fz.command_off,
//             fz.command_move, fz.command_stop, fz.battery.
//   exposes [battery, action(recall_*, on, off, brightness_stop,
//            brightness_move_up, brightness_move_down)].
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. ZB-5004
//   is a remote (sender), not a sink — corrected to action+battery.
#include "definitions/rgb_genie/_shared.hpp"

namespace zhc::devices::rgb_genie {
namespace {
constexpr const char* kModels_ZB_5004[] = { "RGBgenie ZB-5004" };
}  // namespace

constexpr Expose kExposes_ZB_5004[] = {
    { "action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0 },
};

extern const PreparedDefinition kDef_ZB_5004{
    .zigbee_models=kModels_ZB_5004,
    .zigbee_models_count=sizeof(kModels_ZB_5004)/sizeof(kModels_ZB_5004[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-5004", .vendor="RGB Genie",
    .meta=nullptr,
    .exposes=kExposes_ZB_5004,
    .exposes_count=sizeof(kExposes_ZB_5004)/sizeof(kExposes_ZB_5004[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzActionBattery, .from_zigbee_count=kFzActionBatteryCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsActionBattery, .bindings_count=kBindingsActionBatteryCount,
};

}  // namespace zhc::devices::rgb_genie
