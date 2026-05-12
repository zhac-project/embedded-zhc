// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: IKEA SYMFONISK sound remote gen 1 (E1744). Rotary + single
// button remote. Rotation produces brightness_move / step events;
// button toggle.
//
// z2m-source: ikea.ts #E1744 — commandsOnOff({commands:["toggle"]}) +
// commandsLevelCtrl({commands:["brightness_move_up","_down","_stop",
// "brightness_step_up","_down"]}) + ikeaBattery.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {

constexpr const char* kModels_E1744[] = { "SYMFONISK Sound Controller" };

constexpr const char* kActions_E1744[] = {
    "toggle",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "brightness_step_up",
    "brightness_step_down",
};

constexpr Expose kExposes_E1744[] = {
    { "action",                 ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_E1744, sizeof(kActions_E1744)/sizeof(kActions_E1744[0]) },
    { "action_rate",            ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "action_step_size",       ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "action_transition_time", ExposeType::Numeric, Access::State,
      "s",     nullptr, nullptr, 0 },
    { "battery",                ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage",                ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_E1744[] = {
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_E1744[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
};

}  // namespace

extern const PreparedDefinition kDef_E1744{
    .zigbee_models            = kModels_E1744,
    .zigbee_models_count      = sizeof(kModels_E1744)/sizeof(kModels_E1744[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "E1744",
    .vendor                   = "IKEA",
    .meta                     = nullptr,
    .exposes                  = kExposes_E1744,
    .exposes_count            = sizeof(kExposes_E1744)/sizeof(kExposes_E1744[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_E1744,
    .from_zigbee_count        = sizeof(kFz_E1744)/sizeof(kFz_E1744[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_E1744,
    .bindings_count           = sizeof(kBindings_E1744)/sizeof(kBindings_E1744[0]),
};

}  // namespace zhc::devices::ikea
