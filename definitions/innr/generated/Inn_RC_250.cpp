// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Innr RC 250 — battery-powered remote (hand-edited).
// z2m-source: innr.ts #RC 250.
//
// fromZigbee = [command_step, command_on, command_off,
//               command_move_to_level, command_move_to_color_temp]
// extend     = [m.battery()]
// Same shape as RC 210 but uses commandStep (short-press) instead of
// move/stop. The auto-generator's on/off + tz pair was incorrect for a
// battery remote.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {
namespace {

constexpr const char* kModels_RC_250[] = { "RC 250" };

constexpr const char* kActions_RC_250[] = {
    "on",
    "off",
    "brightness_step_up",
    "brightness_step_down",
    "brightness_move_to_level",
    "color_temperature_move",
};

constexpr Expose kExposes_RC_250[] = {
    { "action",  ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_RC_250, sizeof(kActions_RC_250)/sizeof(kActions_RC_250[0]) },
    { "battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_RC_250[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_RC_250[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0008 },  // genLevelCtrl
    { 1, 0x0300 },  // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_RC_250{
    .zigbee_models=kModels_RC_250, .zigbee_models_count=sizeof(kModels_RC_250)/sizeof(kModels_RC_250[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RC 250", .vendor="Innr",
    .meta=nullptr,
    .exposes=kExposes_RC_250, .exposes_count=sizeof(kExposes_RC_250)/sizeof(kExposes_RC_250[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RC_250, .from_zigbee_count=sizeof(kFz_RC_250)/sizeof(kFz_RC_250[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_RC_250, .bindings_count=sizeof(kBindings_RC_250)/sizeof(kBindings_RC_250[0]),
};

}  // namespace zhc::devices::innr
