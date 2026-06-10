// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Evn ZBHS4RGBW — graduated from generated/ for a parity fix.
// Zigbee 4 channel RGBW remote control (battery, multi-EP).
// z2m-source: evn.ts #ZBHS4RGBW.
//
// Parity fix (was a settable on/off switch): the auto-generated def
// wrong-bundled this battery remote as a controllable relay — kFzOnOff +
// kTzOnOff + a writable `state` expose — so every button press was dead
// and the device falsely advertised an on/off endpoint. z2m's role is
// `action` only: the remote is the CLIENT of genOnOff / genLevelCtrl /
// lightingColorCtrl and has `toZigbee: []` (cannot be controlled).
//
// z2m fromZigbee: fz.battery, fz.command_move_to_color,
//   fz.command_move_to_color_temp, fz.command_move_hue, fz.command_step,
//   fz.command_stop, fz.command_move, fz.command_recall, fz.command_on,
//   fz.command_off.
// z2m exposes: e.battery(), e.action([color_move, color_temperature_move,
//   brightness_step_up/down, brightness_move_up/down, brightness_stop,
//   hue_move, hue_stop, recall_*, on, off]).
// z2m meta: {multiEndpoint: true, battery: {dontDividePercentage: true}}.
// z2m endpoint(): {ep1:1, ep2:2, ep3:3, ep4:4}.
//
// multiEndpoint + an explicit endpoint() map → z2m postfixes every action
// with the endpoint name (utils.ts postfixWithEndpointName), so the four
// channels emit distinct actions (e.g. `on_ep3`). Mirrored with
// endpoint_map {ep1..ep4} + endpoint_action_suffix=true so each channel
// keeps identity; `battery` stays a global key (never suffixed).
//
// (dontDividePercentage is a no-op in ZHC: kFzBattery emits whole-percent
//  from the half-percent 0x0021 attr the same way regardless — sibling
//  rgb_genie ZB-5028 / vesternet REM-013 ports document the same.)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::evn {
namespace {
const FzConverter* const kFz_ZBHS4RGBW[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandMoveHue,
    &::zhc::generic::kFzCommandMoveToColor,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZBHS4RGBW[] = { "ZBHS4RGBW" };

constexpr ::zhc::EndpointLabel kEndpoints_ZBHS4RGBW[] = {
    { "ep1", 1 }, { "ep2", 2 }, { "ep3", 3 }, { "ep4", 4 },
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
    {1, 0x0001},   // genPowerCfg (battery)
};

extern const PreparedDefinition kDef_ZBHS4RGBW{
    .zigbee_models=kModels_ZBHS4RGBW, .zigbee_models_count=sizeof(kModels_ZBHS4RGBW)/sizeof(kModels_ZBHS4RGBW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZBHS4RGBW", .vendor="Evn",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZBHS4RGBW, .from_zigbee_count=sizeof(kFz_ZBHS4RGBW)/sizeof(kFz_ZBHS4RGBW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_ZBHS4RGBW,
    .endpoint_map_count=sizeof(kEndpoints_ZBHS4RGBW)/sizeof(kEndpoints_ZBHS4RGBW[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::evn
