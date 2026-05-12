// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Innr RC 110 — 6-endpoint scene remote (hand-edited, PARTIAL).
// z2m-source: innr.ts #RC 110.
//
// fromZigbee = [command_step, command_move, command_stop, command_on,
//               command_off, rc_110_level_to_scene]
// meta       = {multiEndpoint: true}, endpoints l1..l6 mapped to ep 3..8.
//
// Coverage caveat: ZHC has no equivalent of `fz.rc_110_level_to_scene`
// today, so the scene_* actions are not emitted. We still wire the
// generic command_* actions on EP1 so on/off/brightness presses surface
// as actions instead of the broken state-toggle the auto-generator
// produced. Multi-endpoint demux is also not modelled — every EP feeds
// the same `action` channel. Tracked under PARTIAL in INNR_PARITY.md.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {
namespace {

constexpr const char* kModels_RC_110[] = { "RC 110" };

constexpr const char* kActions_RC_110[] = {
    "on",
    "off",
    "brightness_step_up",
    "brightness_step_down",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
};

constexpr Expose kExposes_RC_110[] = {
    { "action", ExposeType::Enum, Access::State,
      nullptr, nullptr,
      kActions_RC_110, sizeof(kActions_RC_110)/sizeof(kActions_RC_110[0]) },
};

const FzConverter* const kFz_RC_110[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
};

constexpr BindingSpec kBindings_RC_110[] = {
    { 1, 0x0006 },  // genOnOff (EP1; multi-EP scene binds blocked).
    { 1, 0x0008 },  // genLevelCtrl
};

}  // namespace

extern const PreparedDefinition kDef_RC_110{
    .zigbee_models=kModels_RC_110, .zigbee_models_count=sizeof(kModels_RC_110)/sizeof(kModels_RC_110[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RC 110", .vendor="Innr",
    .meta=nullptr,
    .exposes=kExposes_RC_110, .exposes_count=sizeof(kExposes_RC_110)/sizeof(kExposes_RC_110[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RC_110, .from_zigbee_count=sizeof(kFz_RC_110)/sizeof(kFz_RC_110[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_RC_110, .bindings_count=sizeof(kBindings_RC_110)/sizeof(kBindings_RC_110[0]),
};

}  // namespace zhc::devices::innr
