// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bitron AV2010/29 — hand-rewritten 2026-04-28.
// Outdoor siren.
// z2m-source: bitron.ts #AV2010/29.
//
// z2m bundle: fz.battery + tz.warning
//   exposes [battery_low, tamper, warning].
//
// Mapped:
//   kFzBattery — genPowerCfg → "battery" / "voltage" / "battery_low".
//   kTzWarning — ssIasWd warning command.
//
// NOTE: previous port also wired kFzIasZone (would have emitted a stray
//   "alarm_1" key). z2m for this SKU only consumes fz.battery; the tamper
//   bit is reported via IAS Zone but the z2m converter is not wired.
//   Removed the IAS converter to match z2m's exact decode bundle.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bitron {
namespace {
const FzConverter* const kFz_AV2010_29[] = {
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_AV2010_29[] = {
    &::zhc::generic::kTzWarning,
};
constexpr const char* kModels_AV2010_29[] = { "AV2010/29", "902010/29" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0502},  // ssIasWd (warning sink)
};

extern const PreparedDefinition kDef_AV2010_29{
    .zigbee_models=kModels_AV2010_29, .zigbee_models_count=sizeof(kModels_AV2010_29)/sizeof(kModels_AV2010_29[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AV2010/29", .vendor="Bitron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AV2010_29, .from_zigbee_count=sizeof(kFz_AV2010_29)/sizeof(kFz_AV2010_29[0]),
    .to_zigbee=kTz_AV2010_29, .to_zigbee_count=sizeof(kTz_AV2010_29)/sizeof(kTz_AV2010_29[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bitron
