// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Insta 57005000 — hand-rewritten 2026-04-28s.
// Switching Actuator Mini with input for wall switch.
// z2m-source: insta.ts #57005000.
//   zigbeeModel: ["NEXENTRO Switching Actuator", "57005000"]
//   fingerprint: manufacturerName "Insta GmbH" + modelID "Generic UP Device"
//                EP1 (genOnOff) + EP4 (wall-switch input) + EP242
//   fromZigbee:  [on_off, command_on, command_off]
//   toZigbee:    [on_off]
//   exposes:     switch()
//   configure:   bind genOnOff on EP1 + report onOff
//
// Mapped:
//   kFzOnOff         — genOnOff attr 0x0000 → state (bool)
//   kFzCommandOn     — genOnOff cmd 0x01   → action="on"  (wall-switch passthrough)
//   kFzCommandOff    — genOnOff cmd 0x00   → action="off"
//   kTzOnOff         — outbound state ON/OFF/TOGGLE → genOnOff cmd 0/1/2
//
// Single-endpoint device — relay state lives on EP1, wall-switch
// input on EP4 sends genOnOff cluster commands. We don't suffix
// `action` per-EP since z2m treats the wall-switch action as a
// single global event stream.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::insta {
namespace {
const FzConverter* const kFz_D57005000[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
};
const TzConverter* const kTz_D57005000[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_D57005000[] = { "NEXENTRO Switching Actuator", "57005000" };

constexpr Expose kAutoExposes[] = {
    {"state",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum,   Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff — main relay
};

}  // namespace

extern const PreparedDefinition kDef_D57005000{
    .zigbee_models=kModels_D57005000, .zigbee_models_count=sizeof(kModels_D57005000)/sizeof(kModels_D57005000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="57005000", .vendor="Insta",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D57005000, .from_zigbee_count=sizeof(kFz_D57005000)/sizeof(kFz_D57005000[0]),
    .to_zigbee=kTz_D57005000, .to_zigbee_count=sizeof(kTz_D57005000)/sizeof(kTz_D57005000[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::insta
