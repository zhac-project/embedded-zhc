// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZS232000178 — hand-rewritten 2026-04-28q.
// 1-key remote control (battery-powered button).
// z2m bundle: fz.command_on + fz.command_off + fz.command_move + fz.command_stop
//             + fz.battery; exposes [action(on/off/brightness_move_up/_down/_stop),
//             battery, battery_low].
// Mapped:
//   kFzCommandOn       — genOnOff cmd 0x01 → action="on".
//   kFzCommandOff      — genOnOff cmd 0x00 → action="off".
//   kFzCommandMove     — genLevelCtrl cmd 0x01 → action="brightness_move_up/_down".
//   kFzCommandStop     — genLevelCtrl cmd 0x03 → action="brightness_stop".
//   kFzBattery         — genPowerCfg battery / voltage.
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. ZS232000178
//   is a sender (button), not a sink — removed.
// z2m-source: linkind.ts #ZS232000178.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::linkind {
namespace {

constexpr const char* kModels_ZS232000178[] = { "ZBT-DIMSwitch-D0001" };

constexpr const char* kActions_ZS232000178[] = {
    "on", "off",
    "brightness_move_up", "brightness_move_down", "brightness_stop",
};

const FzConverter* const kFz_ZS232000178[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr,
      kActions_ZS232000178,
      sizeof(kActions_ZS232000178)/sizeof(kActions_ZS232000178[0]) },
    { "battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
    { "battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
};

extern const PreparedDefinition kDef_ZS232000178{
    .zigbee_models=kModels_ZS232000178, .zigbee_models_count=sizeof(kModels_ZS232000178)/sizeof(kModels_ZS232000178[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZS232000178", .vendor="Linkind",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZS232000178, .from_zigbee_count=sizeof(kFz_ZS232000178)/sizeof(kFz_ZS232000178[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::linkind
