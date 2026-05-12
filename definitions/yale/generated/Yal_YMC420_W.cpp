// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yale YMC420-W — hand-rewritten (was wrong-bundle: had onOff).
// Digital Lock YMC 420 W — closuresDoorLock + battery; ymc_action runtime gap.
// z2m-source: yale.ts #YMC420-W.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yale {
namespace {
const FzConverter* const kFz_YMC420_W[] = {
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_YMC420_W[] = {
    &::zhc::generic::kTzLock,
};
constexpr const char* kModels_YMC420_W[] = { "YMC420", "YMC 420", "YMC420 W", "YMC 420 W" };

}  // namespace


// --- hand-authored exposes (do NOT regenerate; lockExtend defaults) ---
constexpr Expose kAutoExposes[] = {
    {"lock_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0101},
    {1, 0x0001},
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_YMC420_W{
    .zigbee_models=kModels_YMC420_W, .zigbee_models_count=sizeof(kModels_YMC420_W)/sizeof(kModels_YMC420_W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YMC420-W", .vendor="Yale",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YMC420_W, .from_zigbee_count=sizeof(kFz_YMC420_W)/sizeof(kFz_YMC420_W[0]),
    .to_zigbee=kTz_YMC420_W, .to_zigbee_count=sizeof(kTz_YMC420_W)/sizeof(kTz_YMC420_W[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yale
