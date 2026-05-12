// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon SIN-4-FP-20 — hand-rewrite (added power_on_behavior).
// Pilot wire heating module.
//
// z2m: `m.onOff({powerOnBehavior:true}) +
//       m.electricityMeter({cluster:'metering'}) + nodonPilotWire(true)`.
// Custom 0xFC00 PilotWire cluster (pilot_wire_mode) is a TODO; the
// rest is wired here.
//
// z2m-source: nodon.ts #SIN-4-FP-20.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SIN_4_FP_20[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_SIN_4_FP_20[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_SIN_4_FP_20[] = { "SIN-4-FP-20" };

}  // namespace


// Hand-aligned to z2m exposes (pilot_wire_mode TODO).
constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr,
        "Controls the behavior when the device is powered on after power loss",
        nullptr, 0, ExposeCategory::Config},
    {"energy",            ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power",             ExposeType::Numeric, Access::State, "W",   nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SIN_4_FP_20{
    .zigbee_models=kModels_SIN_4_FP_20, .zigbee_models_count=sizeof(kModels_SIN_4_FP_20)/sizeof(kModels_SIN_4_FP_20[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SIN-4-FP-20", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SIN_4_FP_20, .from_zigbee_count=sizeof(kFz_SIN_4_FP_20)/sizeof(kFz_SIN_4_FP_20[0]),
    .to_zigbee=kTz_SIN_4_FP_20, .to_zigbee_count=sizeof(kTz_SIN_4_FP_20)/sizeof(kTz_SIN_4_FP_20[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
