// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZS130000078 — hand-rewritten 2026-04-28q.
// Security keypad (battery, ssIasZone + ssIasAce).
// z2m bundle: fz.command_arm_with_transaction + fz.battery
//             + fz.ias_ace_occupancy_with_timeout + fz.ias_smoke_alarm_1
//             + fz.command_panic + tz.arm_mode + m.iasGetPanelStatusResponse.
// Mapped:
//   kFzBattery        — genPowerCfg battery / voltage.
//   kFzIasSmokeAlarm  — ssIasZone status_change → alarm_1=smoke + tamper + battery_low.
// PARTIAL: no generic decoders yet for ssIasAce `arm` (cmd 0x00 with transaction
//   payload), ssIasAce `panic` (cmd 0x04), nor ssIasAce
//   `ias_ace_occupancy_with_timeout`. Action surface (panic / disarm / arm_*,
//   action_code, action_transaction, action_zone) and tz.arm_mode write path
//   are exposed for SPA but require runtime work to flow through. Tracked
//   under linkind parity doc.
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. The keypad
//   is a sender + IAS source, not an on/off sink — removed.
// z2m-source: linkind.ts #ZS130000078.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::linkind {
namespace {

constexpr const char* kModels_ZS130000078[] = { "ZB-KeypadGeneric-D0002" };

constexpr const char* kActions_ZS130000078[] = {
    "panic", "disarm", "arm_day_zones", "arm_all_zones",
    "exit_delay", "entry_delay",
};

const FzConverter* const kFz_ZS130000078[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasSmokeAlarm,
    &::zhc::generic::kFzIasAceArm,
    &::zhc::generic::kFzIasAcePanic,
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
    { "battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "smoke",       ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr,
      kActions_ZS130000078,
      sizeof(kActions_ZS130000078)/sizeof(kActions_ZS130000078[0]) },
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
    {1, 0x0501},  // ssIasAce
};

extern const PreparedDefinition kDef_ZS130000078{
    .zigbee_models=kModels_ZS130000078, .zigbee_models_count=sizeof(kModels_ZS130000078)/sizeof(kModels_ZS130000078[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZS130000078", .vendor="Linkind",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZS130000078, .from_zigbee_count=sizeof(kFz_ZS130000078)/sizeof(kFz_ZS130000078[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::linkind
