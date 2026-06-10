// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Livolo TI0001-pir — occupancy sensor.
// Zigbee motion Sensor
// z2m-source: livolo.ts #TI0001-pir — exposes e.occupancy(); decode via
// fz.livolo_pir_state over raw genPowerCfg. The auto-port carried a
// phantom IAS bundle (alarm/tamper/battery_low + ssIasZone 0x0500
// binding) that the custom decoder never feeds, while the real
// "occupancy" key had no expose. Corrected to occupancy + genPowerCfg
// binding.
#include "definitions/_generic/_shared.hpp"
#include "definitions/livolo/_shared.hpp"

namespace zhc::devices::livolo {
namespace {
const FzConverter* const kFz_TI0001_pir[] = {
    &::zhc::devices::livolo::kFzLivoloPirState,
};

constexpr const char* kModels_TI0001_pir[] = { "TI0001-pir" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
};

extern const PreparedDefinition kDef_TI0001_pir{
    .zigbee_models=kModels_TI0001_pir, .zigbee_models_count=sizeof(kModels_TI0001_pir)/sizeof(kModels_TI0001_pir[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TI0001-pir", .vendor="Livolo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TI0001_pir, .from_zigbee_count=sizeof(kFz_TI0001_pir)/sizeof(kFz_TI0001_pir[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::livolo
