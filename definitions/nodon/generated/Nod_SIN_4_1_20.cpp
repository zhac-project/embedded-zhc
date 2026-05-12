// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon SIN-4-1-20 — hand-rewrite (added manuSpec writes).
// Multifunction relay switch.
//
// z2m: `m.onOff() + nodonModernExtend.impulseMode() +
//       nodonModernExtend.switchTypeOnOff()`.
// The two NodOn extends are manuSpec writes on `genOnOff` (manu 0x128B):
//   attr 0x0001 u16  → impulse_mode_configuration (ms)
//   attr 0x1001 enum → switch_type {bistable,monostable,auto_detect}
//
// z2m-source: nodon.ts #SIN-4-1-20.
#include "definitions/_generic/_shared.hpp"
#include "definitions/nodon/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SIN_4_1_20[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_SIN_4_1_20[] = {
    &::zhc::generic::kTzOnOff,
    &kTzNodonImpulseMode,
    &kTzNodonSwitchTypeOnOff,
};
constexpr const char* kModels_SIN_4_1_20[] = { "SIN-4-1-20" };

}  // namespace


// Hand-aligned to z2m exposes.
constexpr Expose kAutoExposes[] = {
    {"state",                      ExposeType::Binary,  Access::StateSet, nullptr,
        nullptr, nullptr, 0},
    {"impulse_mode_configuration", ExposeType::Numeric, Access::StateSet, "ms",
        "Set the impulse duration in milliseconds (0 = disabled).",
        nullptr, 0, ExposeCategory::Config},
    {"switch_type",                ExposeType::Enum,    Access::StateSet, nullptr,
        "Switch type: bistable, monostable or auto-detect.",
        nullptr, 0, ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SIN_4_1_20{
    .zigbee_models=kModels_SIN_4_1_20, .zigbee_models_count=sizeof(kModels_SIN_4_1_20)/sizeof(kModels_SIN_4_1_20[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SIN-4-1-20", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SIN_4_1_20, .from_zigbee_count=sizeof(kFz_SIN_4_1_20)/sizeof(kFz_SIN_4_1_20[0]),
    .to_zigbee=kTz_SIN_4_1_20, .to_zigbee_count=sizeof(kTz_SIN_4_1_20)/sizeof(kTz_SIN_4_1_20[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
