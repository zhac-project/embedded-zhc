// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: DawonDns TH-110-ZB — hand-rewritten parity port.
// IoT SMART temperature and humidity sensor.
// z2m: fz.humidity, fz.temperature, fz.battery; bindings on
// msTemperatureMeasurement (0x0402), msRelativeHumidity (0x0405),
// genPowerCfg (0x0001).
// z2m-source: dawon_dns.ts #TH-110-ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::dawon_dns {
namespace {

const FzConverter* const kFz_TH_110_ZB[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_TH_110_ZB[] = { "ZB30C2" };

constexpr Expose kExposes_TH_110_ZB[] = {
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_TH_110_ZB[] = {
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
    { 1, 0x0001 },  // genPowerCfg
};

}  // namespace

extern const PreparedDefinition kDef_TH_110_ZB{
    .zigbee_models=kModels_TH_110_ZB, .zigbee_models_count=sizeof(kModels_TH_110_ZB)/sizeof(kModels_TH_110_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TH-110-ZB", .vendor="DawonDns",
    .meta=nullptr, .exposes=kExposes_TH_110_ZB, .exposes_count=sizeof(kExposes_TH_110_ZB)/sizeof(kExposes_TH_110_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TH_110_ZB, .from_zigbee_count=sizeof(kFz_TH_110_ZB)/sizeof(kFz_TH_110_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindings_TH_110_ZB,
    .bindings_count           = sizeof(kBindings_TH_110_ZB)/sizeof(kBindings_TH_110_ZB[0]),
};

}  // namespace zhc::devices::dawon_dns
