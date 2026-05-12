// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Climax KP-23EL-ZBS-ACE — typed-IAS audit 2026-04-29a.
// Remote keypad — z2m fz: ias_keypad + battery + command_arm + command_panic +
// command_emergency. ZHC wires kFzBattery + kFzIasZone (alarm/tamper/battery_low,
// fills the ias_keypad role) + kFzIasAceArm (action + action_code + action_zone)
// + kFzIasAcePanic (action="panic"). command_emergency has no typed converter
// in zhc::generic yet — see CLIMAX_PARITY.md PARTIAL note. Spurious kFzOnOff +
// kTzOnOff + state expose dropped (z2m has no fz.on_off / tz.on_off here).
// z2m-source: climax.ts #KP-23EL-ZBS-ACE.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::climax {
namespace {
const FzConverter* const kFz_KP_23EL_ZBS_ACE[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
    &::zhc::generic::kFzIasAceArm,
    &::zhc::generic::kFzIasAcePanic,
    &::zhc::generic::kFzIasAceEmergency,
};
constexpr const char* kModels_KP_23EL_ZBS_ACE[] = { "KP-ACE_00.00.03.12TC", "KP-ACE_00.00.03.11TC" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0501},
};

extern const PreparedDefinition kDef_KP_23EL_ZBS_ACE{
    .zigbee_models=kModels_KP_23EL_ZBS_ACE, .zigbee_models_count=sizeof(kModels_KP_23EL_ZBS_ACE)/sizeof(kModels_KP_23EL_ZBS_ACE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="KP-23EL-ZBS-ACE", .vendor="Climax",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_KP_23EL_ZBS_ACE, .from_zigbee_count=sizeof(kFz_KP_23EL_ZBS_ACE)/sizeof(kFz_KP_23EL_ZBS_ACE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::climax
