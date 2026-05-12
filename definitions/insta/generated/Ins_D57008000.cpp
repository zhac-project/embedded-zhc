// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Insta 57008000 — hand-rewritten 2026-04-28s.
// Blinds actor with lift/tilt calibration + inputs for wall switches.
// z2m-source: insta.ts #57008000.
//   zigbeeModel: ["NEXENTRO Blinds Actuator", "Generic UP Device"]
//   fromZigbee:  [cover_position_tilt, command_cover_open,
//                 command_cover_close, command_cover_stop]
//   toZigbee:    [cover_state, cover_position_tilt]
//   exposes:     cover_position_tilt()  — position + tilt, no `state`
//   endpoint:    (device) => ({default: 6})
//   configure:   bind closuresWindowCovering on EP6 + EP7 + report
//                lift/tilt percentage on EP6 (10s settle gap)
//
// Mapped:
//   kFzCoverPosition         — closuresWindowCovering attr 0x0008/0x0009
//                              → position + tilt (raw 0..100)
//   kFzCommandCoverOpen      — cmd 0x00 → action="open"  (wall-switch input)
//   kFzCommandCoverClose     — cmd 0x01 → action="close"
//   kFzCommandCoverStop      — cmd 0x02 → action="stop"
//   kTzCoverState            — outbound state=OPEN/CLOSE/STOP
//   kTzCoverPositionLift     — outbound position=0..100 (cmd 0x05)
//
// Bindings: closuresWindowCovering on EP 6 (position) + EP 7 (action
// passthrough from a wired wall-switch input).
//
// Known parity gaps (documented in INSTA_PARITY.md):
//   * z2m sets `device.powerSource = "Mains (single phase)"` after
//     bind to override the unknown power-source attribute. ZHC has
//     no equivalent runtime hook; the WebUI's power-source label
//     stays "unknown" until the user edits the device.
//   * z2m's configure runs `await utils.sleep(10000)` before bind
//     to give the device firmware time to settle. ZHC's declarative
//     config_steps support this via a `Wait` op — not wired here
//     because we currently bind via the legacy `bindings[]` path,
//     which fires at join with no settle gap. If pairing fails,
//     promote bindings to a config_steps pipeline with a leading
//     Wait.
//   * Tilt setter (cmd 0x08 goToTiltPercentage) — not yet exposed by
//     the runtime. Lift-only setter via kTzCoverPositionLift covers
//     90 % of use; tilt remains read-only via kFzCoverPosition.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::insta {
namespace {
const FzConverter* const kFz_D57008000[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
};
const TzConverter* const kTz_D57008000[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPositionLift,
    &::zhc::generic::kTzCoverPositionTilt,
};
constexpr const char* kModels_D57008000[] = { "NEXENTRO Blinds Actuator", "Generic UP Device" };

constexpr Expose kAutoExposes[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"tilt",     ExposeType::Numeric, Access::State,    "%", nullptr, nullptr, 0},
    {"action",   ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {6, 0x0102},  // closuresWindowCovering — main lift/tilt EP
    {7, 0x0102},  // closuresWindowCovering — wall-switch input EP
};

}  // namespace

extern const PreparedDefinition kDef_D57008000{
    .zigbee_models=kModels_D57008000, .zigbee_models_count=sizeof(kModels_D57008000)/sizeof(kModels_D57008000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="57008000", .vendor="Insta",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D57008000, .from_zigbee_count=sizeof(kFz_D57008000)/sizeof(kFz_D57008000[0]),
    .to_zigbee=kTz_D57008000, .to_zigbee_count=sizeof(kTz_D57008000)/sizeof(kTz_D57008000[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::insta
