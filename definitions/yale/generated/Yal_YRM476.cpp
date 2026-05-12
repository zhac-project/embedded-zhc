// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yale YRM476 — hand-rewritten (was missing kFzLock/kTzLock).
// Assure lock — m.battery() + m.lock(pinCodeCount: 250).
// z2m-source: yale.ts #YRM476.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yale {
namespace {
const FzConverter* const kFz_YRM476[] = {
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_YRM476[] = {
    &::zhc::generic::kTzLock,
};
constexpr const char* kModels_YRM476[] = { "YRM476 TS BLE" };

}  // namespace


// --- hand-authored exposes (do NOT regenerate) ---
constexpr Expose kAutoExposes[] = {
    {"lock_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0101},
    {1, 0x0001},
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_YRM476{
    .zigbee_models=kModels_YRM476, .zigbee_models_count=sizeof(kModels_YRM476)/sizeof(kModels_YRM476[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YRM476", .vendor="Yale",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YRM476, .from_zigbee_count=sizeof(kFz_YRM476)/sizeof(kFz_YRM476[0]),
    .to_zigbee=kTz_YRM476, .to_zigbee_count=sizeof(kTz_YRM476)/sizeof(kTz_YRM476[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yale
