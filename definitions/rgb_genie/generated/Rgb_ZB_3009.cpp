// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: RGB Genie ZB-3009 — hand-rewritten 2026-04-28q.
// 3 scene remote and dimmer (mains, single-EP).
// z2m-source: rgb_genie.ts #ZB-3009.
//
// z2m bundle: same fz list as ZB-3008 but no multiEndpoint flag.
//   exposes [action(...)].
// NOTE: previous port wired kFzOnOff and exposed `state`. ZB-3009 is a
//   mains-powered colour scene remote (sender). PARTIAL: command_move_hue,
//   command_move_to_color, command_move_color_temperature are not yet
//   wired in _generic.
#include "definitions/rgb_genie/_shared.hpp"

namespace zhc::devices::rgb_genie {
namespace {
constexpr const char* kModels_ZB_3009[] = { "RGBgenie ZB-3009" };
}  // namespace

constexpr Expose kExposes_ZB_3009[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0 },
};

extern const PreparedDefinition kDef_ZB_3009{
    .zigbee_models=kModels_ZB_3009,
    .zigbee_models_count=sizeof(kModels_ZB_3009)/sizeof(kModels_ZB_3009[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-3009", .vendor="RGB Genie",
    .meta=nullptr,
    .exposes=kExposes_ZB_3009,
    .exposes_count=sizeof(kExposes_ZB_3009)/sizeof(kExposes_ZB_3009[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzActionRGBNoBattery, .from_zigbee_count=kFzActionRGBNoBatteryCount,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsActionRGBNoBattery, .bindings_count=kBindingsActionRGBNoBatteryCount,
};

}  // namespace zhc::devices::rgb_genie
