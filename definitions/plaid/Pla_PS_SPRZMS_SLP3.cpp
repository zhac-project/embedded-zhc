// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Plaid Systems PS-SPRZMS-SLP3 — Spruce temperature & moisture sensor.
//
// z2m-source: plaid.ts #PS-SPRZMS-SLP3.
//
// Graduated from generated/ to fix dropped temperature + humidity
// channels. z2m wires fromZigbee:[fz.temperature, fz.humidity,
// fz.plaid_battery] and exposes humidity + temperature + battery +
// battery_voltage. The auto-port instead emitted a phantom on/off bundle
// (kFzOnOff + a `state` expose + a 0x0006 genOnOff bind) and battery-only,
// dropping BOTH the temperature (msTemperatureMeasurement 0x0402) and
// humidity (msRelativeHumidity 0x0405) sensor channels — the device's
// primary function. There is no genOnOff on this battery sensor.
//
// Single-endpoint device on EP 1. Wires:
//   - msTemperatureMeasurement → temperature            [generic kFzTemperature, /100]
//   - msRelativeHumidity       → humidity               [generic kFzHumidity, /100]
//   - genPowerCfg mainsVoltage → voltage + battery       [shared  kFzPlaidBattery]
//
// Battery: z2m fz.plaid_battery reads `mainsVoltage` (genPowerCfg attr
// 0x0000, raw mV) — NOT batteryVoltage — and derives `battery` (%) from
// meta.battery.voltageToPercentage {min: 2500, max: 3000}. The generic
// kFzBattery only reads attrs 0x0020/0x0021/0x0035 and would never decode
// this device, so the bespoke kFzPlaidBattery lives in _shared.cpp.
#include "definitions/_generic/_shared.hpp"
#include "definitions/plaid/_shared.hpp"

namespace zhc::devices::plaid {
namespace {


constexpr const char* kModels_PS_SPRZMS_SLP3[] = { "PS-SPRZMS-SLP3" };

const FzConverter* const kFz_PS_SPRZMS_SLP3[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &kFzPlaidBattery,
};

constexpr Expose kExp_PS_SPRZMS_SLP3[] = {
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "°C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
};

constexpr BindingSpec kBind_PS_SPRZMS_SLP3[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_PS_SPRZMS_SLP3{
    .zigbee_models=kModels_PS_SPRZMS_SLP3, .zigbee_models_count=sizeof(kModels_PS_SPRZMS_SLP3)/sizeof(kModels_PS_SPRZMS_SLP3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PS-SPRZMS-SLP3", .vendor="Plaid",
    .meta=nullptr, .exposes=kExp_PS_SPRZMS_SLP3, .exposes_count=sizeof(kExp_PS_SPRZMS_SLP3)/sizeof(kExp_PS_SPRZMS_SLP3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PS_SPRZMS_SLP3, .from_zigbee_count=sizeof(kFz_PS_SPRZMS_SLP3)/sizeof(kFz_PS_SPRZMS_SLP3[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_PS_SPRZMS_SLP3,
    .bindings_count           = sizeof(kBind_PS_SPRZMS_SLP3)/sizeof(kBind_PS_SPRZMS_SLP3[0]),
};

}  // namespace zhc::devices::plaid
