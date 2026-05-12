// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SMA03P — hand-rewritten (added temperature + humidity).
// Environmental sensor.
//
// z2m: fromZigbee:[fz.battery, fz.temperature, fz.humidity]
//   exposes: battery, voltage, temperature, humidity
//
// 2026-04-28p MERCATOR sweep — generator dropped the fz.temperature
// and fz.humidity decoders.
//
// z2m-source: mercator.ts #SMA03P.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SMA03P[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_SMA03P[] = { "TS0201" };
constexpr const char* kManus_SMA03P[] = { "_TZ3000_82ptnsd4" };

constexpr Expose kExposes_SMA03P[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV",  nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State, "°C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SMA03P[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_SMA03P{
    .zigbee_models=kModels_SMA03P, .zigbee_models_count=sizeof(kModels_SMA03P)/sizeof(kModels_SMA03P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SMA03P, .manufacturer_names_count=sizeof(kManus_SMA03P)/sizeof(kManus_SMA03P[0]),
    .model="SMA03P", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SMA03P, .exposes_count=sizeof(kExposes_SMA03P)/sizeof(kExposes_SMA03P[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SMA03P, .from_zigbee_count=sizeof(kFz_SMA03P)/sizeof(kFz_SMA03P[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SMA03P, .bindings_count=sizeof(kBindings_SMA03P)/sizeof(kBindings_SMA03P[0]),
};

}  // namespace zhc::devices::mercator
