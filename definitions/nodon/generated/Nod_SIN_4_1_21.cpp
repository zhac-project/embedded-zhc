// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon SIN-4-1-21 — hand-rewrite (added PoB + manuSpec writes).
// Multifunction relay switch with metering.
//
// z2m: `m.onOff({powerOnBehavior:true}) +
//       m.electricityMeter({cluster:'metering'}) +
//       nodonModernExtend.impulseMode() + nodonModernExtend.switchTypeOnOff()`.
//
// z2m-source: nodon.ts #SIN-4-1-21.
#include "definitions/_generic/_shared.hpp"
#include "definitions/nodon/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SIN_4_1_21[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_SIN_4_1_21[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
    &kTzNodonImpulseMode,
    &kTzNodonSwitchTypeOnOff,
};
constexpr const char* kModels_SIN_4_1_21[] = { "SIN-4-1-21" };

}  // namespace


// Hand-aligned to z2m exposes.
constexpr Expose kAutoExposes[] = {
    {"state",                      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior",          ExposeType::Enum,    Access::StateSet, nullptr,
        "Controls the behavior when the device is powered on after power loss",
        nullptr, 0, ExposeCategory::Config},
    {"energy",                     ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power",                      ExposeType::Numeric, Access::State, "W",   nullptr, nullptr, 0},
    {"impulse_mode_configuration", ExposeType::Numeric, Access::StateSet, "ms",
        "Set the impulse duration in milliseconds (0 = disabled).",
        nullptr, 0, ExposeCategory::Config},
    {"switch_type",                ExposeType::Enum,    Access::StateSet, nullptr,
        "Switch type: bistable, monostable or auto-detect.",
        nullptr, 0, ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SIN_4_1_21{
    .zigbee_models=kModels_SIN_4_1_21, .zigbee_models_count=sizeof(kModels_SIN_4_1_21)/sizeof(kModels_SIN_4_1_21[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SIN-4-1-21", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SIN_4_1_21, .from_zigbee_count=sizeof(kFz_SIN_4_1_21)/sizeof(kFz_SIN_4_1_21[0]),
    .to_zigbee=kTz_SIN_4_1_21, .to_zigbee_count=sizeof(kTz_SIN_4_1_21)/sizeof(kTz_SIN_4_1_21[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
