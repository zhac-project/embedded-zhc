// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Somfy 1871154 — hand-rewritten 2026-04-29e; suffix fix 2026-06-10.
// Ysia 5 channel blinds remote. z2m uses `m.commandsOnOff` +
// `m.commandsWindowCovering`, BOTH with endpointNames ["1".."5"] —
// each command is postfixed with its endpoint (on_1, open_2, ...).
// Wire the command-receiver Fz pipeline (action exposes), not the
// kFzOnOff/kTzOnOff state-set pipeline. No to_zigbee — there is no
// local relay or motor on the device.
//
// PARITY FIX: the def carried an endpoint_map (EPs 1..5) but NOT
// `endpoint_action_suffix`, so dispatch kept "action" in
// kAlwaysGlobalKeys (bare) and all five channels collided on one
// unsuffixed action key — the per-button identity z2m encodes via
// postfixWithEndpointName was lost. Setting the flag makes dispatch
// rewrite action -> action_<n> (ez convention; z2m suffixes the
// value, ez suffixes the key — same information, established here).
// z2m-source: somfy.ts #1871154.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::somfy {
namespace {
const FzConverter* const kFz_D1871154[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D1871154[] = { "Ysia 5 HP Zigbee" };

constexpr ::zhc::EndpointLabel kEndpoints_D1871154[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5} };

}  // namespace


// --- hand-edited exposes/bindings ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

// Per-EP binds for genOnOff and closuresWindowCovering on EPs 1..5;
// battery (genPowerCfg) reports from EP1.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006}, {2, 0x0006}, {3, 0x0006}, {4, 0x0006}, {5, 0x0006},
    {1, 0x0102}, {2, 0x0102}, {3, 0x0102}, {4, 0x0102}, {5, 0x0102},
};
// --- end hand-edited block ---

extern const PreparedDefinition kDef_D1871154{
    .zigbee_models=kModels_D1871154, .zigbee_models_count=sizeof(kModels_D1871154)/sizeof(kModels_D1871154[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1871154", .vendor="Somfy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D1871154, .from_zigbee_count=sizeof(kFz_D1871154)/sizeof(kFz_D1871154[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_D1871154,
    .endpoint_map_count = sizeof(kEndpoints_D1871154)/sizeof(kEndpoints_D1871154[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::somfy
