// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Innr RC 210 — battery-powered remote (hand-edited).
// z2m-source: innr.ts #RC 210.
//
// fromZigbee = [command_on, command_off, command_move, command_stop,
//               command_move_to_level, command_move_to_color_temp]
// extend     = [m.battery()]
// The auto-generator treats this as an on/off device because it sees
// genOnOff bindings; the device is actually a battery remote that emits
// cluster-specific commands. We replace the on/off fz/tz with the
// matching command_* action decoders and re-shape exposes.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {
namespace {

constexpr const char* kModels_RC_210[] = { "RC 210" };

constexpr const char* kActions_RC_210[] = {
    "on",
    "off",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "brightness_move_to_level",
    "color_temperature_move",
};

constexpr Expose kExposes_RC_210[] = {
    { "action",  ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_RC_210, sizeof(kActions_RC_210)/sizeof(kActions_RC_210[0]) },
    { "battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_RC_210[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_RC_210[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0008 },  // genLevelCtrl
    { 1, 0x0300 },  // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_RC_210{
    .zigbee_models=kModels_RC_210, .zigbee_models_count=sizeof(kModels_RC_210)/sizeof(kModels_RC_210[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RC 210", .vendor="Innr",
    .meta=nullptr,
    .exposes=kExposes_RC_210, .exposes_count=sizeof(kExposes_RC_210)/sizeof(kExposes_RC_210[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RC_210, .from_zigbee_count=sizeof(kFz_RC_210)/sizeof(kFz_RC_210[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_RC_210, .bindings_count=sizeof(kBindings_RC_210)/sizeof(kBindings_RC_210[0]),
};

}  // namespace zhc::devices::innr
