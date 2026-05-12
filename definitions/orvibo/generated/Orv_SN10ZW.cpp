// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo SN10ZW.
// Occupancy sensor
// z2m-source: orvibo.ts #SN10ZW.
// z2m uses ias_occupancy_alarm_1_with_timeout (auto-clear). ZHC's
// kFzIasMotionAlarm decodes the same wire but doesn't auto-clear — the
// rules engine or shadow can debounce. Functional parity, no auto-reset
// timer.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_SN10ZW[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};
constexpr const char* kModels_SN10ZW[] = { "131c854783bc45c9b2ac58088d09571c", "b2e57a0f606546cd879a1a54790827d6", "585fdfb8c2304119a2432e9845cf2623" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_SN10ZW{
    .zigbee_models=kModels_SN10ZW, .zigbee_models_count=sizeof(kModels_SN10ZW)/sizeof(kModels_SN10ZW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SN10ZW", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SN10ZW, .from_zigbee_count=sizeof(kFz_SN10ZW)/sizeof(kFz_SN10ZW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
