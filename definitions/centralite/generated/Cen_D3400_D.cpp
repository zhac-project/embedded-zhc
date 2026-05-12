// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Centralite 3400-D — hand-rewritten 2026-04-28.
// 3-Series security keypad.
// z2m-source: centralite.ts #3400-D.
//
// z2m bundle:
//   fz.command_arm_with_transaction       — ssIasAce arm command → "action"
//   fz.temperature                         — msTemperatureMeasurement
//   fz.battery                             — genPowerCfg
//   fz.ias_ace_occupancy_with_timeout      — derives "occupancy"
//   tz.arm_mode                            — outbound arm responses
//
// Mapped:
//   kFzTemperature, kFzBattery — fully covered.
//
// Runtime gap: ssIasAce (0x0501) command-driven action / occupancy / arm-mode
//   write paths are not yet plumbed in ZHC's generic shared layer. The keypad
//   reports temperature + battery via this port; arming actions, action_code,
//   action_zone, action_transaction, and `tz.arm_mode` writes remain unwired
//   (single-device need; below the shared-converter threshold). The previous
//   auto-generated bundle wired `kFzOnOff` + `kFzIasZone` which the device
//   does not actually report — those have been removed.
//   See docs/CENTRALITE_PARITY.md.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::centralite {
namespace {
const FzConverter* const kFz_D3400_D[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D3400_D[] = { "3400-D", "3400" };

constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // occupancy / action / action_code / action_zone / action_transaction —
    // pending ssIasAce (0x0501) command + arm-mode wiring.
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone (kept for join hand-shake)
    {1, 0x0501},  // ssIasAce  (kept for join hand-shake)
};
}  // namespace

extern const PreparedDefinition kDef_D3400_D{
    .zigbee_models=kModels_D3400_D, .zigbee_models_count=sizeof(kModels_D3400_D)/sizeof(kModels_D3400_D[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3400-D", .vendor="Centralite",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3400_D, .from_zigbee_count=sizeof(kFz_D3400_D)/sizeof(kFz_D3400_D[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::centralite
