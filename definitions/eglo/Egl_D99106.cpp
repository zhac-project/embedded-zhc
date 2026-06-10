// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Eglo 99106 (TLSR82xx) — Connect-Z motion (PIR) sensor remote, not a switch.
//
// Despite the "motion sensor" description, z2m models it as a command
// transmitter (it sends genOnOff/genLevelCtrl/lightingColorCtrl commands to
// bound lights, not IAS/occupancy reports):
//   fromZigbee: [fz.command_on, fz.command_move_to_level,
//                fz.command_move_to_color_temp]
//   exposes: e.action(["on","brightness_move_to_level","color_temperature_move"])
// The generated def wired kFzOnOff + an on/off TZ (both dead) and exposed a
// meaningless `state`. Re-wired to the generic command-action decoders so the
// stream surfaces as `action`.
//
// z2m-source: eglo.ts #99106.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::eglo {
namespace {
const FzConverter* const kFz_D99106[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
};
constexpr const char* kModels_D99106[] = { "TLSR82xx" };

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0300},
};
}  // namespace

extern const PreparedDefinition kDef_D99106{
    .zigbee_models=kModels_D99106, .zigbee_models_count=sizeof(kModels_D99106)/sizeof(kModels_D99106[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="99106", .vendor="Eglo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D99106, .from_zigbee_count=sizeof(kFz_D99106)/sizeof(kFz_D99106[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::eglo
