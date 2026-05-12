// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Hive KEYPAD001 — hand-tuned.
// Alarm security keypad — z2m wiring is fz.command_arm_with_transaction +
// fz.command_panic + fz.battery + fz.ias_occupancy_alarm_1 + fz.identify
// + fz.ias_contact_alarm_1 + fz.ias_ace_occupancy_with_timeout, with
// tz.arm_mode and exposes for action codes / arm zones / panic.
//
// The ZHC generic library currently exposes the IAS-Zone and battery
// paths but NOT the IAS-ACE arm/panic command converters or fz.identify,
// so this port is PARTIAL: the keypad's status/tamper/battery reports
// reach the shadow, but PIN-coded arm/disarm actions and the IAS-ACE
// panel-status reply path are not wired. Earlier auto-port wrongly
// listed kFzOnOff/kTzOnOff and bound 0x0006 (the keypad has no genOnOff
// reporting). Cleared on/off, restored the binds the device's configure
// actually performs (genPowerCfg + ssIasZone + ssIasAce + genIdentify).
// z2m-source: hive.ts #KEYPAD001.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hive {
namespace {
const FzConverter* const kFz_KEYPAD001[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
};
constexpr const char* kModels_KEYPAD001[] = { "KEYPAD001" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
    {1, 0x0501},  // ssIasAce
    {1, 0x0003},  // genIdentify
};

extern const PreparedDefinition kDef_KEYPAD001{
    .zigbee_models=kModels_KEYPAD001, .zigbee_models_count=sizeof(kModels_KEYPAD001)/sizeof(kModels_KEYPAD001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="KEYPAD001", .vendor="Hive",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_KEYPAD001, .from_zigbee_count=sizeof(kFz_KEYPAD001)/sizeof(kFz_KEYPAD001[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hive
