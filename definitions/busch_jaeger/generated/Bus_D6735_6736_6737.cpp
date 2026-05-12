// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: BuschJaeger 6735/6736/6737 — hand-rewritten 2026-04-28s.
// Zigbee Light Link power supply / relay / dimmer / wall-switch.
// Models: 6735 (1-row), 6736 (2-row), 6737 (4-row); battery-backed
// variants 6735/01, 6736/01, 6737/01 share zigbeeModel "RB01".
// z2m-source: busch_jaeger.ts #6735/6736/6737.
//   fromZigbee: [on_off, brightness, command_on, command_off,
//                command_step, command_stop, command_recall]
//   toZigbee:   [light_onoff_brightness, light_brightness_step,
//                light_brightness_move]
//   endpoint:   {row_1: 0x0a, row_2: 0x0b, row_3: 0x0c, row_4: 0x0d,
//                relay: 0x12}
//   exposes:    light_brightness().withEndpoint("relay") (when EP 18 present)
//               action(off_row_<n>/on_row_<n>/
//                      brightness_step_{up,down}_row_<n>/
//                      brightness_stop_row_<n>/recall_*_row_*)
//   meta:       multiEndpoint
//
// Mapped:
//   kFzOnOff             — relay state on EP 18 → state_relay
//   kFzBrightness        — relay level on EP 18 → brightness_relay
//   kFzCommandOn         — genOnOff cmd 0x01 from rocker EP → action "on"
//   kFzCommandOff        — genOnOff cmd 0x00 → action "off"
//   kFzCommandStep       — genLevelCtrl cmd 0x02 → "brightness_step_up/down"
//   kFzCommandStop       — genLevelCtrl cmd 0x03 → "brightness_stop"
//   kFzCommandRecall     — genScenes cmd 0x05 → "recall_<scene>"
//   kTzOnOff / kTzBrightness — drive the relay/dimmer back-end on EP 18
//
// Multi-endpoint:
//   endpoint_map labels EP 10..13 as row_1..row_4 and EP 18 as relay.
//   endpoint_action_suffix=true so action keys gain `_row_<n>` /
//   `_relay` suffix matching z2m's enumeration. State / brightness
//   reports from EP 18 become `state_relay` / `brightness_relay`.
//
// Known parity gaps (documented in BUSCH_JAEGER_PARITY.md):
//   * z2m runs a 60s poll on EP 18 because the device doesn't report
//     genOnOff / genLevelCtrl. ZHC has no built-in poller (`m.poll`
//     extend has no runtime equivalent) so state changes made by
//     external means (manual toggle, scene recall) won't be reflected
//     until an explicit read. Needs a generic poll runtime.
//   * The bind set in z2m skips EP 10 when EP 18 (relay) is present
//     and conditionally binds EP 11/12/13 only when those rockers
//     exist. We declare the maximal binding set; the device may
//     reject extra binds with a soft failure (≈5-binding limit per
//     z2m's comment) — harmless but visible in logs.
//   * `recall_*_row_*` labels: z2m emits e.g. `recall_3_row_2`
//     (scene id 3 fired from row 2). Our `kFzCommandRecall` emits
//     `recall_<scene>`; the suffix dispatcher then appends
//     `_row_<n>` based on the source EP, yielding `recall_<n>_row_<n>`.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::busch_jaeger {
namespace {

const FzConverter* const kFz_D6735_6736_6737[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
};

const TzConverter* const kTz_D6735_6736_6737[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};

constexpr const char* kModels_D6735_6736_6737[] = { "RM01", "RB01" };

constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",     ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {18, 0x0006},  // genOnOff      — relay/dimmer back-end (EP 18)
    {18, 0x0008},  // genLevelCtrl  — relay/dimmer back-end (EP 18)
    {10, 0x0008},  // genLevelCtrl  — rocker row 1 (EP 10)
    {11, 0x0008},  // genLevelCtrl  — rocker row 2 (EP 11)
    {12, 0x0008},  // genLevelCtrl  — rocker row 3 (EP 12)
    {13, 0x0008},  // genLevelCtrl  — rocker row 4 (EP 13)
};

constexpr ::zhc::EndpointLabel kEndpoints_D6735_6736_6737[] = {
    {"row_1", 10},
    {"row_2", 11},
    {"row_3", 12},
    {"row_4", 13},
    {"relay", 18},
};

}  // namespace

extern const PreparedDefinition kDef_D6735_6736_6737{
    .zigbee_models=kModels_D6735_6736_6737, .zigbee_models_count=sizeof(kModels_D6735_6736_6737)/sizeof(kModels_D6735_6736_6737[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="6735/6736/6737", .vendor="BuschJaeger",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D6735_6736_6737, .from_zigbee_count=sizeof(kFz_D6735_6736_6737)/sizeof(kFz_D6735_6736_6737[0]),
    .to_zigbee=kTz_D6735_6736_6737, .to_zigbee_count=sizeof(kTz_D6735_6736_6737)/sizeof(kTz_D6735_6736_6737[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_D6735_6736_6737,
    .endpoint_map_count=sizeof(kEndpoints_D6735_6736_6737)/sizeof(kEndpoints_D6735_6736_6737[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::busch_jaeger
