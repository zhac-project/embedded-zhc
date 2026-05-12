// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZS130000178 — hand-rewritten 2026-04-28q.
// Security system key fob (battery sender, no on/off sink).
// z2m bundle: fz.command_arm + fz.command_panic + m.iasArmCommandDefaultResponse;
//             exposes [action(panic/disarm/arm_partial_zones/arm_all_zones)].
// Updated 2026-04-28r post-sweep: kFzIasAceArm + kFzIasAcePanic landed in
//   _generic/_shared. Button presses now flow to shadow as
//   action=<panic|disarm|arm_*|exit_delay|entry_delay> + action_code + action_zone.
// NOTE: previous port wired kFzOnOff and exposed `state`. ZS130000178 is a
//   battery key fob, not a sink — removed.
// z2m-source: linkind.ts #ZS130000178.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::linkind {
namespace {

constexpr const char* kModels_ZS130000178[] = { "ZB-KeyfodGeneric-D0001" };

constexpr const char* kActions_ZS130000178[] = {
    "panic", "disarm", "arm_partial_zones", "arm_all_zones",
};

// Fz wired to the new _generic kFzIasAceArm + kFzIasAcePanic.
const FzConverter* const kFz_ZS130000178[] = {
    &::zhc::generic::kFzIasAceArm,
    &::zhc::generic::kFzIasAcePanic,
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr,
      kActions_ZS130000178,
      sizeof(kActions_ZS130000178)/sizeof(kActions_ZS130000178[0]) },
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0501},  // ssIasAce — emit binding so device targets coordinator
};

extern const PreparedDefinition kDef_ZS130000178{
    .zigbee_models=kModels_ZS130000178, .zigbee_models_count=sizeof(kModels_ZS130000178)/sizeof(kModels_ZS130000178[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZS130000178", .vendor="Linkind",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZS130000178, .from_zigbee_count=sizeof(kFz_ZS130000178)/sizeof(kFz_ZS130000178[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::linkind
