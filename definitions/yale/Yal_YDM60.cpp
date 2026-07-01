// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Yale YDM60 door lock (z2m v26.76.0 parity).
// z2m-source: yale.ts #YDM60
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yale {
namespace {
// z2m: fromZigbee [fzLocal.ymc_action], extend [lockExtend()].
// lockExtend() bundles fz.lock + fz.battery; ymc_action is a custom raw
// closuresDoorLock action decoder with no generic equivalent — the action
// expose is a runtime gap and is dropped, matching the identical-shape
// SOLIS01 / YMC420-W sibling ports.
const FzConverter* const kFz_YDM60[] = {
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_YDM60[] = {
    &::zhc::generic::kTzLock,
};
constexpr const char* kModels_YDM60[] = { "YDM60" };

}  // namespace


// --- hand-authored exposes (lockExtend defaults; ymc_action action dropped) ---
constexpr Expose kAutoExposes[] = {
    {"lock_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0101},
    {1, 0x0001},
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_YDM60{
    .zigbee_models=kModels_YDM60, .zigbee_models_count=sizeof(kModels_YDM60)/sizeof(kModels_YDM60[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YDM60", .vendor="Yale",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YDM60, .from_zigbee_count=sizeof(kFz_YDM60)/sizeof(kFz_YDM60[0]),
    .to_zigbee=kTz_YDM60, .to_zigbee_count=sizeof(kTz_YDM60)/sizeof(kTz_YDM60[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yale
