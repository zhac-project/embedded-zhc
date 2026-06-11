// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Smart9 S9ZGBRC01 — IAS ACE action audit 2026-06-11.
// Smart remote controller (TS0215) — battery key fob / panic remote.
// z2m bundle: fz.command_arm + fz.command_emergency + fz.battery,
//   exposes e.battery() + e.action([disarm, arm_day_zones, arm_night_zones,
//   arm_all_zones, exit_delay, emergency]). toZigbee: []. configure binds
//   genPowerCfg only. extend m.iasArmCommandDefaultResponse() = link-layer
//   default-response handler (no decode surface).
// Mapped:
//   kFzBattery         — genPowerCfg battery / voltage.
//   kFzIasAceArm       — ssIasAce commandArm (cmd 0x00) → action="<armMode>"
//                        (disarm/arm_day_zones/arm_night_zones/arm_all_zones/
//                        exit_delay/...) + action_code + action_zone.
//   kFzIasAceEmergency — ssIasAce commandEmergency (cmd 0x02) → action="emergency".
// NOTE: previous auto-port wired kFzOnOff/kTzOnOff + a phantom `state` Binary
//   StateSet expose + a spurious genOnOff(0x0006) bind. This device is an IAS
//   ACE sender (key fob), not an on/off sink — the arm/emergency actions were
//   dead and the controllable state was a phantom. Replaced with the IAS ACE
//   action surface; bind narrowed to genPowerCfg(0x0001) to match z2m configure.
//   z2m uses fz.command_emergency (commandEmergency, cmd 0x02), NOT command_panic
//   (cmd 0x04) — so kFzIasAceEmergency, not kFzIasAcePanic.
// z2m-source: smart9.ts #S9ZGBRC01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smart9 {
namespace {
const FzConverter* const kFz_S9ZGBRC01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasAceArm,
    &::zhc::generic::kFzIasAceEmergency,
};
constexpr const char* kModels_S9ZGBRC01[] = { "TS0215" };

constexpr const char* kActions_S9ZGBRC01[] = {
    "disarm", "arm_day_zones", "arm_night_zones",
    "arm_all_zones", "exit_delay", "emergency",
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActions_S9ZGBRC01,
     sizeof(kActions_S9ZGBRC01)/sizeof(kActions_S9ZGBRC01[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
};

extern const PreparedDefinition kDef_S9ZGBRC01{
    .zigbee_models=kModels_S9ZGBRC01, .zigbee_models_count=sizeof(kModels_S9ZGBRC01)/sizeof(kModels_S9ZGBRC01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S9ZGBRC01", .vendor="Smart9",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S9ZGBRC01, .from_zigbee_count=sizeof(kFz_S9ZGBRC01)/sizeof(kFz_S9ZGBRC01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smart9
