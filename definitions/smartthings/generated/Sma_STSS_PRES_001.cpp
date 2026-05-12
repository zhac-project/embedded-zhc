// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings STSS-PRES-001 — parity-extended 2026-04-28.
// Presence sensor (original "PGC410EU"/"PGC410").
// z2m-source: smartthings.ts #STSS-PRES-001.
//
// z2m bundle: presence (fzLocal.PGC410EU_presence — manuSpecific
// SmartThings ArrivalSensor cluster, attr 0x0000 in manu 0x110A) + battery.
//
// Runtime gap: manuSpecificSmartThingsArrivalSensor (0xFC05, manu 0x110A)
// presence attribute is not yet wired into ZHC's static cluster table.
// Only `battery`/`voltage` will be emitted until then.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_STSS_PRES_001[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_STSS_PRES_001[] = { "PGC410EU", "PGC410" };

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // presence — pending manuSpecificSmartThingsArrivalSensor cluster wiring.
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
};
}  // namespace

extern const PreparedDefinition kDef_STSS_PRES_001{
    .zigbee_models=kModels_STSS_PRES_001, .zigbee_models_count=sizeof(kModels_STSS_PRES_001)/sizeof(kModels_STSS_PRES_001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STSS-PRES-001", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STSS_PRES_001, .from_zigbee_count=sizeof(kFz_STSS_PRES_001)/sizeof(kFz_STSS_PRES_001[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
