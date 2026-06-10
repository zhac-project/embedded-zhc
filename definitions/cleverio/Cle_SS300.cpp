// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Cleverio SS300 — temperature/humidity sensor.
// z2m-source: cleverio.ts #SS300.
//   fz: fz.temperature (msTemperatureMeasurement 0x0402 measuredValue /100),
//       fz.humidity (msRelativeHumidity 0x0405 measuredValue /100),
//       fz.battery (genPowerCfg 0x0001).
//   tz: tz.battery_percentage_remaining.
//
// Parity fix (z2m<->embedded-zhc pass): the generated def wired ONLY
// kFzBattery and exposed only battery/voltage — it dropped the two primary
// sensor channels (temperature + humidity) that z2m decodes via
// fz.temperature + fz.humidity. Re-added the generic kFzTemperature /
// kFzHumidity converters, the matching exposes, and the msTemperature /
// msRelativeHumidity bindings.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::cleverio {
namespace {
const FzConverter* const kFz_SS300[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SS300[] = { "SM0201" };
constexpr const char* kManus_SS300[] = { "_TYZB01_lzrhtcxu" };

constexpr Expose kExp_SS300[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",  nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_SS300[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
};
}  // namespace

extern const PreparedDefinition kDef_SS300{
    .zigbee_models=kModels_SS300, .zigbee_models_count=sizeof(kModels_SS300)/sizeof(kModels_SS300[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SS300, .manufacturer_names_count=sizeof(kManus_SS300)/sizeof(kManus_SS300[0]),
    .model="SS300", .vendor="Cleverio",
    .meta=nullptr, .exposes=kExp_SS300, .exposes_count=sizeof(kExp_SS300)/sizeof(kExp_SS300[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SS300, .from_zigbee_count=sizeof(kFz_SS300)/sizeof(kFz_SS300[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SS300, .bindings_count=sizeof(kBind_SS300)/sizeof(kBind_SS300[0]),
};

}  // namespace zhc::devices::cleverio
