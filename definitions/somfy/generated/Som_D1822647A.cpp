// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Somfy 1822647A — hand-rewritten 2026-04-29e.
// Zigbee smart plug. z2m's `configure` binds endpoint **12** (not 1)
// for genOnOff + seMetering. Outbound on/off must therefore route to
// EP12, so we override `default_endpoint` and re-key bindings to ep12.
// z2m-source: somfy.ts #1822647A.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::somfy {
namespace {
const FzConverter* const kFz_D1822647A[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_D1822647A[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_D1822647A[] = { "1822647" };

}  // namespace


// --- hand-edited bindings (z2m binds ep12) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {12, 0x0006},
    {12, 0x0702},
};
// --- end hand-edited block ---

extern const PreparedDefinition kDef_D1822647A{
    .zigbee_models=kModels_D1822647A, .zigbee_models_count=sizeof(kModels_D1822647A)/sizeof(kModels_D1822647A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1822647A", .vendor="Somfy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D1822647A, .from_zigbee_count=sizeof(kFz_D1822647A)/sizeof(kFz_D1822647A[0]),
    .to_zigbee=kTz_D1822647A, .to_zigbee_count=sizeof(kTz_D1822647A)/sizeof(kTz_D1822647A[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .default_endpoint=12,
};

}  // namespace zhc::devices::somfy
