// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Immax 07767L — hand-rewritten (TS004F battery scene button).
// NEO Smart outdoor button
// z2m-source: immax.ts #07767L.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::immax {
namespace {
const FzConverter* const kFz_D07767L[] = {
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D07767L[] = { "TS004F" };
constexpr const char* kManus_D07767L[] = { "_TZ3000_krwtzhfd" };
}  // namespace


constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0005},
    {1, 0x0001},
};

extern const PreparedDefinition kDef_D07767L{
    .zigbee_models=kModels_D07767L, .zigbee_models_count=sizeof(kModels_D07767L)/sizeof(kModels_D07767L[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_D07767L, .manufacturer_names_count=sizeof(kManus_D07767L)/sizeof(kManus_D07767L[0]),
    .model="07767L", .vendor="Immax",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D07767L, .from_zigbee_count=sizeof(kFz_D07767L)/sizeof(kFz_D07767L[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::immax
