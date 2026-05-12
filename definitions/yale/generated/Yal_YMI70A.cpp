// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yale YMI70A — hand-rewritten (was missing kFzLock/kTzLock).
// Biometric digital lock — m.battery() + m.lock(pinCodeCount: 100).
// z2m-source: yale.ts #YMI70A.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yale {
namespace {
const FzConverter* const kFz_YMI70A[] = {
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_YMI70A[] = {
    &::zhc::generic::kTzLock,
};
constexpr const char* kModels_YMI70A[] = { "06e0152204" };

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

extern const PreparedDefinition kDef_YMI70A{
    .zigbee_models=kModels_YMI70A, .zigbee_models_count=sizeof(kModels_YMI70A)/sizeof(kModels_YMI70A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YMI70A", .vendor="Yale",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YMI70A, .from_zigbee_count=sizeof(kFz_YMI70A)/sizeof(kFz_YMI70A[0]),
    .to_zigbee=kTz_YMI70A, .to_zigbee_count=sizeof(kTz_YMI70A)/sizeof(kTz_YMI70A[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yale
