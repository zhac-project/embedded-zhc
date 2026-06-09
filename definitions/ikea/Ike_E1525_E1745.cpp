// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ikea E1525/E1745 — TRADFRI motion sensor.
//
// z2m-source: ikea.ts #E1525/E1745 — tradfriOccupancy() +
// tradfriRequestedBrightness() + m.identify() + ikeaBattery().
//
// The auto-generated def wired the full kFzIkeaLight bundle — a phantom
// light on a battery PIR. Replaced with the generic battery decoder
// (which matches z2m's ikeaBattery() genPowerCfg path).
//
// NOT yet wired (no generic decoder exists — INFRA, deferred): IKEA's
// older motion sensor does NOT report msOccupancySensing. Instead it
// *binds outward* and sends genOnOff commandOnWithTimedOff (occupancy)
// and genLevelCtrl commandMoveToLevelWithOnOff (requested brightness)
// to its bound target. Decoding those into `occupancy` /
// `requested_brightness_*` requires command converters with a publish-
// false timer (tradfriOccupancy) that do not exist generically yet.
// Tracked as a separate infra ticket. (Note the newer VALLHORN/E2134
// uses real msOccupancySensing and IS fully wired.)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {
const FzConverter* const kFz_E1525_E1745[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_E1525_E1745[] = { "TRADFRI motion sensor" };

constexpr Expose kExposes_E1525_E1745[] = {
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_E1525_E1745[] = {
    {1, 0x0001},   // genPowerCfg
};

}  // namespace

extern const PreparedDefinition kDef_E1525_E1745{
    .zigbee_models=kModels_E1525_E1745, .zigbee_models_count=sizeof(kModels_E1525_E1745)/sizeof(kModels_E1525_E1745[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E1525/E1745", .vendor="Ikea",
    .meta=nullptr, .exposes=kExposes_E1525_E1745, .exposes_count=sizeof(kExposes_E1525_E1745)/sizeof(kExposes_E1525_E1745[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E1525_E1745, .from_zigbee_count=sizeof(kFz_E1525_E1745)/sizeof(kFz_E1525_E1745[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E1525_E1745, .bindings_count=sizeof(kBindings_E1525_E1745)/sizeof(kBindings_E1525_E1745[0]),
};

}  // namespace zhc::devices::ikea
