// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lincukoo SZT06 — temperature/humidity sensor (parent override).
// Smart mini temperature and humidity sensor.
//
// z2m-source: lincukoo.ts #SZT06 —
//   extend: [m.temperature(), m.humidity(), m.identify({isSleepy:true}),
//            m.battery({voltage:true})]
//
// Bug fixed: the generated stub lowered only kFzBattery and exposed only
// battery/voltage. SZT06 reports its primary measurements on the standard
// msTemperatureMeasurement (0x0402) and msRelativeHumidity (0x0405)
// clusters (z2m m.temperature()/m.humidity()), so a paired sensor surfaced
// no temperature or humidity at all. Re-point at kFzTemperature + kFzHumidity
// (which emit the "temperature"/"humidity" keys) and add the 0x0402 / 0x0405
// reporting bindings alongside the existing 0x0001 power binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {
const FzConverter* const kFz_SZT06[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SZT06[] = { "SZT06" };

constexpr Expose kExp_SZT06[] = {
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_SZT06[] = {
    { 1, 0x0001 }, // genPowerCfg
    { 1, 0x0402 }, // msTemperatureMeasurement
    { 1, 0x0405 }, // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_SZT06{
    .zigbee_models=kModels_SZT06, .zigbee_models_count=sizeof(kModels_SZT06)/sizeof(kModels_SZT06[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SZT06", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_SZT06, .exposes_count=sizeof(kExp_SZT06)/sizeof(kExp_SZT06[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SZT06, .from_zigbee_count=sizeof(kFz_SZT06)/sizeof(kFz_SZT06[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SZT06, .bindings_count=sizeof(kBind_SZT06)/sizeof(kBind_SZT06[0]),
};

}  // namespace zhc::devices::lincukoo
