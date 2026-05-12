// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E13-A21 — hand-rewritten 2026-04-28.
// Flood light (PAR38) with motion sensor light outdoor
// z2m-source: sengled.ts #E13-A21.
// Mapping: m.identify() + sengledLight({color: false}) + electricityMeter(metering)
//          + manuSpecificSengledMotionSensor (custom cluster 0xfc01).
//          -> Light bundle + per-port FZ/binding for seMetering. The custom
//          motion-sensor cluster (trigger_condition / enable_auto_on_off /
//          motion_status / off_delay) is BLOCKED in ZHC: it requires
//          deviceAddCustomCluster and manufacturer-coded read/write helpers
//          that the static-memory runtime does not currently expose.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {

const ::zhc::FzConverter* const kFz_E13_A21[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
};

constexpr const char* kModels_E13_A21[] = { "E13-A21" };

constexpr Expose kExposes_E13_A21[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "energy",     ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "kWh",   nullptr, nullptr, 0 },
    { "power",      ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "W",     nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_E13_A21[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0702 },
};

}  // namespace

extern const PreparedDefinition kDef_E13_A21{
    .zigbee_models=kModels_E13_A21, .zigbee_models_count=sizeof(kModels_E13_A21)/sizeof(kModels_E13_A21[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E13-A21", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposes_E13_A21, .exposes_count=sizeof(kExposes_E13_A21)/sizeof(kExposes_E13_A21[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E13_A21, .from_zigbee_count=sizeof(kFz_E13_A21)/sizeof(kFz_E13_A21[0]),
    .to_zigbee=kTzSengledLight, .to_zigbee_count=kTzSengledLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E13_A21, .bindings_count=sizeof(kBindings_E13_A21)/sizeof(kBindings_E13_A21[0]),
};

}  // namespace zhc::devices::sengled
