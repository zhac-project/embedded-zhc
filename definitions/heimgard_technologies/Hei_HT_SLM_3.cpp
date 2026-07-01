// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HeimgardTechnologies HT-SLM-3 — Entry (PIN/RFID) door lock (z2m v26.76.0 parity).
// z2m-source: heimgard_technologies.ts #HT-SLM-3
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heimgard_technologies {
namespace {
// z2m fromZigbee: [fz.lock, fz.battery, fz.lock_pin_code_response,
// fz.lock_user_status_response]. lock_pin_code_response has no generic
// equivalent (read-back confirmation only) — dropped, as in the HT-SLM-2 /
// HC-SLM-1 sibling ports.
const FzConverter* const kFz_HT_SLM_3[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzLockUserStatusResponse,
};
// z2m toZigbee: [tz.lock, tz.lock_sound_volume, tz.identify, tz.pincode_lock,
// tz.lock_userstatus]. tz.identify and tz.lock_userstatus have no generic
// equivalents — dropped, matching the sibling locks.
const TzConverter* const kTz_HT_SLM_3[] = {
    &::zhc::generic::kTzLock,
    &::zhc::generic::kTzLockPinCode,
    &::zhc::generic::kTzLockSoundVolume,
};
constexpr const char* kModels_HT_SLM_3[] = { "HT-SLM-3" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"lock_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sound_volume", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pin_code", ExposeType::String, Access::Set, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0101},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_HT_SLM_3{
    .zigbee_models=kModels_HT_SLM_3, .zigbee_models_count=sizeof(kModels_HT_SLM_3)/sizeof(kModels_HT_SLM_3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HT-SLM-3", .vendor="HeimgardTechnologies",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HT_SLM_3, .from_zigbee_count=sizeof(kFz_HT_SLM_3)/sizeof(kFz_HT_SLM_3[0]),
    .to_zigbee=kTz_HT_SLM_3, .to_zigbee_count=sizeof(kTz_HT_SLM_3)/sizeof(kTz_HT_SLM_3[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heimgard_technologies
