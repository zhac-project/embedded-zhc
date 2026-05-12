// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HeimgardTechnologies HC-SLM-1 — Wattle door lock pro.
// Generic lock + battery + lock programming/operation events + PIN
// code encoder + sound_volume write (landed 2026-04-28s via _generic).
// z2m-source: heimgard_technologies.ts #HC-SLM-1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heimgard_technologies {
namespace {
const FzConverter* const kFz_HC_SLM_1[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzLockUserStatusResponse,
    &::zhc::generic::kFzLockProgrammingEvent,
    &::zhc::generic::kFzLockOperationEvent,
};
const TzConverter* const kTz_HC_SLM_1[] = {
    &::zhc::generic::kTzLock,
    &::zhc::generic::kTzLockPinCode,
    &::zhc::generic::kTzLockSoundVolume,
    &::zhc::generic::kTzLockAutoRelockTime,
};
constexpr const char* kModels_HC_SLM_1[] = { "HC-SLM-1" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"lock_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sound_volume", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pin_code", ExposeType::String, Access::Set, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_user", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source_name", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0101},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_HC_SLM_1{
    .zigbee_models=kModels_HC_SLM_1, .zigbee_models_count=sizeof(kModels_HC_SLM_1)/sizeof(kModels_HC_SLM_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HC-SLM-1", .vendor="HeimgardTechnologies",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HC_SLM_1, .from_zigbee_count=sizeof(kFz_HC_SLM_1)/sizeof(kFz_HC_SLM_1[0]),
    .to_zigbee=kTz_HC_SLM_1, .to_zigbee_count=sizeof(kTz_HC_SLM_1)/sizeof(kTz_HC_SLM_1[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heimgard_technologies
