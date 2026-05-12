// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Konke TW-S1 — IAS photoelectric smoke detector (typed).
// z2m-source: konke.ts #TW-S1 (fz.ias_smoke_alarm_1).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::konke {
namespace {
const FzConverter* const kFz_TW_S1[] = {
    &::zhc::generic::kFzIasSmokeAlarm,
};

constexpr const char* kModels_TW_S1[] = { "3AFE010104020028", "LH05121" };

constexpr Expose kAutoExposes[] = {
    {"smoke",       ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_TW_S1{
    .zigbee_models=kModels_TW_S1, .zigbee_models_count=sizeof(kModels_TW_S1)/sizeof(kModels_TW_S1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TW-S1", .vendor="Konke",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TW_S1, .from_zigbee_count=sizeof(kFz_TW_S1)/sizeof(kFz_TW_S1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::konke
