// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo W45CZ.
// Smart curtain motor
// z2m-source: orvibo.ts #W45CZ.
// Same partial as W40CZ — no analogInput fallback.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_W45CZ[] = {
    &::zhc::generic::kFzCoverPosition,
};
const TzConverter* const kTz_W45CZ[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kModels_W45CZ[] = { "428f8caf93574815be1a98fa6732c3ea" };

}  // namespace

constexpr const char* kEnum_W45CZ_1[] = { "OPEN", "CLOSE", "STOP" };

constexpr Expose kAutoExposes[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kEnum_W45CZ_1, 3},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},
};

extern const PreparedDefinition kDef_W45CZ{
    .zigbee_models=kModels_W45CZ, .zigbee_models_count=sizeof(kModels_W45CZ)/sizeof(kModels_W45CZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="W45CZ", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_W45CZ, .from_zigbee_count=sizeof(kFz_W45CZ)/sizeof(kFz_W45CZ[0]),
    .to_zigbee=kTz_W45CZ, .to_zigbee_count=sizeof(kTz_W45CZ)/sizeof(kTz_W45CZ[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
