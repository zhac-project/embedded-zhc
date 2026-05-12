// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon PIR313-E — auto-generated.
// Motion sensor
// z2m-source: owon.ts #PIR313-E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz_PIR313_E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_PIR313_E[] = { "PIR313-E", "PIR313" };

}  // namespace


// z2m: occupancy + tamper + battery_low + temperature + humidity + illuminance.
// Temp/humidity reporting binds on ep2 (PIR313-E) or ep3 (PIR313); we bind ep2 by default.
constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
    {2, 0x0402},  // msTemperatureMeasurement
    {2, 0x0405},  // msRelativeHumidity
    {2, 0x0400},  // msIlluminanceMeasurement
};

extern const PreparedDefinition kDef_PIR313_E{
    .zigbee_models=kModels_PIR313_E, .zigbee_models_count=sizeof(kModels_PIR313_E)/sizeof(kModels_PIR313_E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PIR313-E", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PIR313_E, .from_zigbee_count=sizeof(kFz_PIR313_E)/sizeof(kFz_PIR313_E[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
