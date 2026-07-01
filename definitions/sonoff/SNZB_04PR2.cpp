// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: hand-authored z2m v26.76.0 parity port (Phase B).
// Sonoff SNZB-04PR2 — Contact sensor (z2m v26.76.0 parity).
//
// z2m-source: sonoff.ts #SNZB-04PR2. Graduated from the generated stub to add
// the battery surface introduced by z2m's `m.battery({voltage: true})` extend
// (delta v26.35->v26.76 added `m:battery`): the stub already wired the
// `kFzBattery` decoder but declared no battery/voltage exposes and bound only
// the IAS zone cluster (0x0500). This adds the `battery` (%) + `voltage` (mV)
// exposes and the genPowerCfg (0x0001) binding to match. contact/tamper/
// battery_low continue to come from the IAS zone-alarm path (0x0500); z2m's
// manu-specific eWeLink 0xFC11 tamper attribute is covered here via the IAS
// zone-status tamper bit, so no custom-cluster converter is added.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
const FzConverter* const kFz_SNZB_04PR2[] = {
    &::zhc::generic::kFzIasZone,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SNZB_04PR2[] = { "SNZB-04PR2" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"alarm", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
    {1, 0x0001},
};

extern const PreparedDefinition kDef_SNZB_04PR2{
    .zigbee_models=kModels_SNZB_04PR2, .zigbee_models_count=sizeof(kModels_SNZB_04PR2)/sizeof(kModels_SNZB_04PR2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNZB-04PR2", .vendor="Sonoff",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SNZB_04PR2, .from_zigbee_count=sizeof(kFz_SNZB_04PR2)/sizeof(kFz_SNZB_04PR2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sonoff
