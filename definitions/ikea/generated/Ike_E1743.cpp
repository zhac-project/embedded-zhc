// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: IKEA TRADFRI on/off switch (E1743). Battery remote with
// short-press on/off + long-press brightness-move dim path.
//
// z2m-source: ikea.ts #E1743 — commandsOnOff + commandsLevelCtrl
// (brightness_move_up/_down/_stop) + ikeaBattery.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {

constexpr const char* kModels_E1743[] = { "TRADFRI on/off switch" };

constexpr const char* kActions_E1743[] = {
    "on",
    "off",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
};

constexpr Expose kExposes_E1743[] = {
    { "action",      ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_E1743, sizeof(kActions_E1743)/sizeof(kActions_E1743[0]) },
    { "action_rate", ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_E1743[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_E1743[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
};

}  // namespace

extern const PreparedDefinition kDef_E1743{
    .zigbee_models            = kModels_E1743,
    .zigbee_models_count      = sizeof(kModels_E1743)/sizeof(kModels_E1743[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "E1743",
    .vendor                   = "IKEA",
    .meta                     = nullptr,
    .exposes                  = kExposes_E1743,
    .exposes_count            = sizeof(kExposes_E1743)/sizeof(kExposes_E1743[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_E1743,
    .from_zigbee_count        = sizeof(kFz_E1743)/sizeof(kFz_E1743[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_E1743,
    .bindings_count           = sizeof(kBindings_E1743)/sizeof(kBindings_E1743[0]),
};

}  // namespace zhc::devices::ikea
