// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: IKEA STYRBAR 4-button remote (E2001/E2002/E2313).
//
// z2m-source: ikea.ts #E2001/E2002/E2313 — styrbarCommandOn +
// commandsOnOff{off} + commandsLevelCtrl{brightness_move_up/down/stop} +
// ikeaArrowClick{styrbar:true} + ikeaBattery.
//
// Buttons: on (top), off (bottom), arrow left/right, brightness
// move-up/down via level-ctrl commands. z2m suppresses a spurious
// genOnOff "on" seen ~500 ms after each arrow release; we do not, since
// our fz converters are stateless. Consumers can de-dupe via a short
// timer if needed.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {

constexpr const char* kModels_E2001_E2002_E2313[] = { "Remote Control N2" };

constexpr const char* kActions_E2001_E2002_E2313[] = {
    "on",
    "off",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "arrow_left_click",
    "arrow_right_click",
    "arrow_left_hold",
    "arrow_right_hold",
    "arrow_release",
};

constexpr Expose kExposes_E2001_E2002_E2313[] = {
    { "action",          ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_E2001_E2002_E2313, sizeof(kActions_E2001_E2002_E2313)/sizeof(kActions_E2001_E2002_E2313[0]) },
    { "action_rate",     ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0 },
    { "action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0 },
    { "battery",         ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
    { "voltage",         ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0 },
};

const FzConverter* const kFz_E2001_E2002_E2313[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzIkeaArrowClick,
    &::zhc::generic::kFzIkeaArrowHold,
    &::zhc::generic::kFzIkeaArrowRelease,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_E2001_E2002_E2313[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
};

}  // namespace

extern const PreparedDefinition kDef_E2001_E2002_E2313{
    .zigbee_models            = kModels_E2001_E2002_E2313,
    .zigbee_models_count      = sizeof(kModels_E2001_E2002_E2313)/sizeof(kModels_E2001_E2002_E2313[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "E2001/E2002/E2313",
    .vendor                   = "IKEA",
    .meta                     = nullptr,
    .exposes                  = kExposes_E2001_E2002_E2313,
    .exposes_count            = sizeof(kExposes_E2001_E2002_E2313)/sizeof(kExposes_E2001_E2002_E2313[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_E2001_E2002_E2313,
    .from_zigbee_count        = sizeof(kFz_E2001_E2002_E2313)/sizeof(kFz_E2001_E2002_E2313[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_E2001_E2002_E2313,
    .bindings_count           = sizeof(kBindings_E2001_E2002_E2313)/sizeof(kBindings_E2001_E2002_E2313[0]),
};

}  // namespace zhc::devices::ikea
