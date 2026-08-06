// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: SONOFF Dongle-PP10, Dongle Plus CC2674P10 with ROUTER firmware
// (z2m v26.92.0 parity, added v26.79.0).
// z2m-source: sonoff.ts #Dongle-PP10
//
// A mains-powered router, not a controllable device: upstream wires only
// fz.linkquality_from_basic (link quality is transport metadata ZHC already
// carries on every frame) and m.forcePowerSource. So this definition exists to
// give the router an identity -- correct model, vendor and power source -- with
// no exposes and no converters, which is exactly what upstream models.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr const char* kModels_Dongle_PP10[] = { "Dongle-PP10" };
}  // namespace

extern const PreparedDefinition kDef_Dongle_PP10{
    .zigbee_models=kModels_Dongle_PP10, .zigbee_models_count=sizeof(kModels_Dongle_PP10)/sizeof(kModels_Dongle_PP10[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Dongle-PP10", .vendor="SONOFF",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .power_source_override=0x01,  // ZCL Basic 0x0007 = Mains (single phase)
};

}  // namespace zhc::devices::sonoff
