// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Terncy TERNCY-LS01 — uses shared terncy converters.
// Smart light socket — genOnOff state set/get plus a single-press
// action emitted via the manuSpecificClusterAduroSmart raw frame.
// z2m's `configure` block binds genOnOff at coordinator-side; we
// declare the binding so the platform handles it the same way.
// z2m-source: terncy.ts #TERNCY-LS01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/terncy/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

const FzConverter* const kFz_TERNCY_LS01[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::terncy::kFzTerncyRawAction,
};
const TzConverter* const kTz_TERNCY_LS01[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_TERNCY_LS01[] = { "TERNCY-LS01" };

constexpr Expose kAutoExposes[] = {
    {"state",  ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff (z2m configure binds this)
    {1, 0xFCCC},   // manuSpecificClusterAduroSmart (raw action)
};

}  // namespace

extern const PreparedDefinition kDef_TERNCY_LS01{
    .zigbee_models=kModels_TERNCY_LS01,
    .zigbee_models_count=sizeof(kModels_TERNCY_LS01)/sizeof(kModels_TERNCY_LS01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-LS01", .vendor="Terncy",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TERNCY_LS01, .from_zigbee_count=sizeof(kFz_TERNCY_LS01)/sizeof(kFz_TERNCY_LS01[0]),
    .to_zigbee=kTz_TERNCY_LS01, .to_zigbee_count=sizeof(kTz_TERNCY_LS01)/sizeof(kTz_TERNCY_LS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::terncy
