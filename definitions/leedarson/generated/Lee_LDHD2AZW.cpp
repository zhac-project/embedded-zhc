// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson LDHD2AZW — hand-rewritten 2026-04-29.
// Magnetic door & window contact sensor (with temperature)
// z2m-source: leedarson.ts #LDHD2AZW.
// Mapping: fromZigbee=[fz.ias_contact_alarm_1, fz.temperature, fz.battery]
//          fz.ias_contact_alarm_1 -> kFzIasContactAlarm.
//          z2m exposes: contact, battery_low, tamper, temperature, battery.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
const FzConverter* const kFz_LDHD2AZW[] = {
    &::zhc::generic::kFzIasContactAlarm,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_LDHD2AZW[] = { "LDHD2AZW" };

constexpr Expose kExposes_LDHD2AZW[] = {
    { "contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State, "°C",   nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_LDHD2AZW[] = {
    { 1, 0x0500 },  // ssIasZone (contact + tamper + battery_low bits)
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0001 },  // genPowerCfg (battery)
};
}  // namespace

extern const PreparedDefinition kDef_LDHD2AZW{
    .zigbee_models=kModels_LDHD2AZW, .zigbee_models_count=sizeof(kModels_LDHD2AZW)/sizeof(kModels_LDHD2AZW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LDHD2AZW", .vendor="Leedarson",
    .meta=nullptr, .exposes=kExposes_LDHD2AZW, .exposes_count=sizeof(kExposes_LDHD2AZW)/sizeof(kExposes_LDHD2AZW[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LDHD2AZW, .from_zigbee_count=sizeof(kFz_LDHD2AZW)/sizeof(kFz_LDHD2AZW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_LDHD2AZW, .bindings_count=sizeof(kBindings_LDHD2AZW)/sizeof(kBindings_LDHD2AZW[0]),
};

}  // namespace zhc::devices::leedarson
