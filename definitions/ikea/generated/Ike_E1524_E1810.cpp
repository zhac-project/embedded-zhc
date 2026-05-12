// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: IKEA TRADFRI 5-button remote (E1524/E1810).
//
// z2m-source: ikea.ts #E1524/E1810 — tradfriCommandsOnOff +
// tradfriCommandsLevelCtrl + ikeaArrowClick + ikeaBattery.
//
// Five buttons: center (toggle), brightness up/down (short = step,
// long = move/stop), arrow left/right (short = click, long = hold,
// release). The IKEA custom genScenes commands are handled by
// kFzIkeaArrow{Click,Hold,Release}.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {

constexpr const char* kModels_E1524_E1810[] = { "TRADFRI remote control" };

constexpr const char* kActions_E1524_E1810[] = {
    "toggle",
    "brightness_step_up",
    "brightness_step_down",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "arrow_left_click",
    "arrow_right_click",
    "arrow_left_hold",
    "arrow_right_hold",
    "arrow_release",
};

constexpr Expose kExposes_E1524_E1810[] = {
    { "action",          ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_E1524_E1810, sizeof(kActions_E1524_E1810)/sizeof(kActions_E1524_E1810[0]) },
    { "action_rate",     ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0 },
    { "action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0 },
    { "battery",         ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
    { "voltage",         ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0 },
};

const FzConverter* const kFz_E1524_E1810[] = {
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzIkeaArrowClick,
    &::zhc::generic::kFzIkeaArrowHold,
    &::zhc::generic::kFzIkeaArrowRelease,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_E1524_E1810[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
    // genScenes (0x0005) is not bound per z2m comment; arrow commands
    // are received as non-group-addressed unicasts.
};

}  // namespace

extern const PreparedDefinition kDef_E1524_E1810{
    .zigbee_models            = kModels_E1524_E1810,
    .zigbee_models_count      = sizeof(kModels_E1524_E1810)/sizeof(kModels_E1524_E1810[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "E1524/E1810",
    .vendor                   = "IKEA",
    .meta                     = nullptr,
    .exposes                  = kExposes_E1524_E1810,
    .exposes_count            = sizeof(kExposes_E1524_E1810)/sizeof(kExposes_E1524_E1810[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_E1524_E1810,
    .from_zigbee_count        = sizeof(kFz_E1524_E1810)/sizeof(kFz_E1524_E1810[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_E1524_E1810,
    .bindings_count           = sizeof(kBindings_E1524_E1810)/sizeof(kBindings_E1524_E1810[0]),
};

}  // namespace zhc::devices::ikea
