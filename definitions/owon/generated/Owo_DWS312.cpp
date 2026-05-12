// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon DWS312 — auto-generated.
// Door/window sensor
// z2m-source: owon.ts #DWS312.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
// z2m: m.battery() + m.iasZoneAlarm({zoneType:"contact", attrs:[alarm_1, battery_low, tamper]}).
const FzConverter* const kFz_DWS312[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_DWS312[] = { "DWS312" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_DWS312{
    .zigbee_models=kModels_DWS312, .zigbee_models_count=sizeof(kModels_DWS312)/sizeof(kModels_DWS312[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DWS312", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DWS312, .from_zigbee_count=sizeof(kFz_DWS312)/sizeof(kFz_DWS312[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
