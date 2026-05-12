// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Centralite 3323-G — hand-rewritten 2026-04-28.
// Micro-door sensor.
// z2m-source: centralite.ts #3323-G.
//
// z2m bundle: fz.ias_contact_alarm_1 + fz.temperature
//   exposes [contact, battery_low, tamper, temperature].
//
// Mapped:
//   kFzIasContactAlarm — ssIasZone status bit 0 → "contact" (inverted: 0=closed),
//                        bit 2 → "tamper", bit 3 → "battery_low"
//   kFzTemperature     — msTemperatureMeasurement → "temperature"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::centralite {
namespace {
const FzConverter* const kFz_D3323_G[] = {
    &::zhc::generic::kFzIasContactAlarm,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_D3323_G[] = { "3323-G" };

constexpr Expose kAutoExposes[] = {
    {"contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
};
}  // namespace

extern const PreparedDefinition kDef_D3323_G{
    .zigbee_models=kModels_D3323_G, .zigbee_models_count=sizeof(kModels_D3323_G)/sizeof(kModels_D3323_G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3323-G", .vendor="Centralite",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3323_G, .from_zigbee_count=sizeof(kFz_D3323_G)/sizeof(kFz_D3323_G[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::centralite
