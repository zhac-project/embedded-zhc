// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HeimgardTechnologies HT-DWM-2 — Door sensor.
// IAS Zone contact alarm (semantic `contact` key) + battery (landed
// 2026-04-28s).
// z2m-source: heimgard_technologies.ts #HT-DWM-2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heimgard_technologies {
namespace {
const FzConverter* const kFz_HT_DWM_2[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_HT_DWM_2[] = { "HT-DWM-2" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_HT_DWM_2{
    .zigbee_models=kModels_HT_DWM_2, .zigbee_models_count=sizeof(kModels_HT_DWM_2)/sizeof(kModels_HT_DWM_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HT-DWM-2", .vendor="HeimgardTechnologies",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HT_DWM_2, .from_zigbee_count=sizeof(kFz_HT_DWM_2)/sizeof(kFz_HT_DWM_2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heimgard_technologies
