// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Terncy TERNCY-SD01 — uses shared terncy converters.
// Knob smart dimmer — emits action (single/double/triple/quadruple)
// from the manuSpecificClusterAduroSmart raw frame and rotate
// (clockwise / counterclockwise + action_number) from the
// terncyRotation attribute (0x001b INT16). z2m exposes
// `direction` as a free-text field; we keep the modern key
// `action_direction`. No outbound writes.
// z2m-source: terncy.ts #TERNCY-SD01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/terncy/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

const FzConverter* const kFz_TERNCY_SD01[] = {
    &::zhc::terncy::kFzTerncyRawAction,
    &::zhc::terncy::kFzTerncyKnob,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_TERNCY_SD01[] = { "TERNCY-SD01" };

constexpr Expose kAutoExposes[] = {
    {"action",           ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_direction", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_number",    ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",          ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage",          ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0xFCCC},   // manuSpecificClusterAduroSmart
};

}  // namespace

extern const PreparedDefinition kDef_TERNCY_SD01{
    .zigbee_models=kModels_TERNCY_SD01,
    .zigbee_models_count=sizeof(kModels_TERNCY_SD01)/sizeof(kModels_TERNCY_SD01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-SD01", .vendor="Terncy",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TERNCY_SD01, .from_zigbee_count=sizeof(kFz_TERNCY_SD01)/sizeof(kFz_TERNCY_SD01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::terncy
