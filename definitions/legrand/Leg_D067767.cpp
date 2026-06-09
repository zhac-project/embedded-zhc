// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand 067767 — batteryless color-dimmer remote, not a switch.
//
// Wireless Color Ambiance switch (067767/68/69, 077710L), no local relay.
// The generated def wired kFzOnOff + an on/off TZ (both dead). The device
// emits genOnOff + genLevelCtrl client->server commands (z2m
// m.commandsOnOff() + m.commandsLevelCtrl()) plus Legrand-specific scene
// recalls. Re-wired to the generic command-action decoders so the on/off
// and brightness button stream surfaces as `action`.
//
// Note: z2m also wires fz.legrand_scenes (a Legrand genScenes-recall
// remap to enter/leave/sleep/wakeup labels). That mapping is
// device-specific and has no generic equivalent, so those scene
// sub-actions are not surfaced here (the on/off + brightness command
// stream is).
//
// z2m-source: legrand.ts #067767 —
//   fromZigbee: [fz.battery, fz.legrand_scenes]
//   extend: [m.battery(), m.commandsOnOff(), m.commandsLevelCtrl()]
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
const FzConverter* const kFz_D067767[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D067767[] = { " Wireless Color Dimmer" };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_D067767{
    .zigbee_models=kModels_D067767, .zigbee_models_count=sizeof(kModels_D067767)/sizeof(kModels_D067767[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="067767", .vendor="Legrand",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D067767, .from_zigbee_count=sizeof(kFz_D067767)/sizeof(kFz_D067767[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::legrand
