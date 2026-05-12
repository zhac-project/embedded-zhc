// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon STPH-4-1-00 — hand-rewrite (added temperature/humidity).
// Temperature & humidity sensor.
//
// z2m: `m.battery() + m.temperature() + m.humidity()`. The original
// auto-generated port shipped with battery only.
//
// z2m-source: nodon.ts #STPH-4-1-00.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_STPH_4_1_00[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_STPH_4_1_00[] = { "STPH-4-1-00" };

}  // namespace


// Hand-aligned to z2m exposes.
constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"temperature", ExposeType::Numeric, Access::State, "C",   nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_STPH_4_1_00{
    .zigbee_models=kModels_STPH_4_1_00, .zigbee_models_count=sizeof(kModels_STPH_4_1_00)/sizeof(kModels_STPH_4_1_00[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STPH-4-1-00", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STPH_4_1_00, .from_zigbee_count=sizeof(kFz_STPH_4_1_00)/sizeof(kFz_STPH_4_1_00[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
