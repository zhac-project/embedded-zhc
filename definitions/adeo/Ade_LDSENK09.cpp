// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Adeo LDSENK09 — hand-maintained parity override.
// Security system key fob
//
// Graduated from generated/Ade_LDSENK09.cpp: the generated def wired
// kFzOnOff + kTzOnOff and exposed a controllable `state`. The key fob has no
// relay — it is an ssIasAce *sender* that emits arm / panic commands
// (client->server). z2m decodes it via fz.command_arm + fz.command_panic and
// exposes `action` [panic, disarm, arm_partial_zones, arm_all_zones]. The
// generated `state` was dead (genOnOff attribute reports never arrive) and the
// action surface was missing entirely. Wired the typed kFzIasAceArm +
// kFzIasAcePanic (ssIasAce cmd 0x00 -> action=<arm_mode> + action_code +
// action_zone; cmd 0x04 -> action="panic") and exposed `action`. Mirrors the
// linkind ZS130000078 / climax KP-23EL-ZBS-ACE key-fob/keypad ports.
// z2m-source: adeo.ts #LDSENK09.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {
const FzConverter* const kFz_LDSENK09[] = {
    &::zhc::generic::kFzIasAceArm,
    &::zhc::generic::kFzIasAcePanic,
};

constexpr const char* kModels_LDSENK09[] = { "LDSENK09" };

constexpr const char* kActions_LDSENK09[] = {
    "panic", "disarm", "arm_partial_zones", "arm_all_zones",
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr,
      kActions_LDSENK09,
      sizeof(kActions_LDSENK09)/sizeof(kActions_LDSENK09[0]) },
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0501},  // ssIasAce
};

extern const PreparedDefinition kDef_LDSENK09{
    .zigbee_models=kModels_LDSENK09, .zigbee_models_count=sizeof(kModels_LDSENK09)/sizeof(kModels_LDSENK09[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LDSENK09", .vendor="Adeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LDSENK09, .from_zigbee_count=sizeof(kFz_LDSENK09)/sizeof(kFz_LDSENK09[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adeo
