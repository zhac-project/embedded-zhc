// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ctm mSwitch_Mic — auto-generated.
// Mikrofon, alarm detection microphone
// z2m-source: ctm.ts #mSwitch_Mic.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
// Mikrofon is battery-powered (no relay) — z2m wires battery,
// command_on/off, ctm_group_config, ias_smoke_alarm_1, temperature.
const FzConverter* const kFz_mSwitch_Mic[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzIasSmokeAlarm,
};
constexpr const char* kModels_mSwitch_Mic[] = { "Mikrofon" };

}  // namespace


// Manual: smoke detector (was generic `alarm`); drop spurious `state`;
// add temperature + action. group_id BLOCKED on ctmGroupConfig manuSpec.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0402},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_mSwitch_Mic{
    .zigbee_models=kModels_mSwitch_Mic, .zigbee_models_count=sizeof(kModels_mSwitch_Mic)/sizeof(kModels_mSwitch_Mic[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="mSwitch_Mic", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_mSwitch_Mic, .from_zigbee_count=sizeof(kFz_mSwitch_Mic)/sizeof(kFz_mSwitch_Mic[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
