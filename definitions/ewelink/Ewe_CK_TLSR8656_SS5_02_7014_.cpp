// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Ewelink CK-TLSR8656-SS5-02(7014) temperature & humidity sensor (z2m v26.76.0 parity).
//
// z2m-source: ewelink.ts #CK-TLSR8656-SS5-02(7014).
// z2m def: extend: [m.temperature(), m.humidity(), m.battery()].
//   m.temperature() -> msTemperatureMeasurement 0x0402 measuredValue /100 -> kFzTemperature
//   m.humidity()    -> msRelativeHumidity      0x0405 measuredValue /100 -> kFzHumidity
//   m.battery()     -> genPowerCfg 0x0001 batteryPercentageRemaining /2  -> kFzBattery
//
// Distinct model from Sonoff SNZB-02's "CK-TLSR8656-SS5-01(7014)" (SS5-*01*, a
// different fingerprint already covered by definitions/sonoff/SNZB_02.cpp); this
// row is SS5-*02*, so a fresh def is required.
//
// Voltage note: z2m's modern m.battery() defaults to voltage=false, so it
// exposes battery% only (NO battery_voltage) -- unlike SNZB-02 / SNZB-02M which
// use old-style fz.battery + an explicit e.battery_voltage(). kFzBattery still
// decodes voltage into the shadow, but it is intentionally NOT advertised here
// to hold strict z2m expose parity.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ewelink {
namespace {
constexpr const char* kModels_CK_TLSR8656_SS5_02_7014[] = { "CK-TLSR8656-SS5-02(7014)" };
constexpr Expose kExposes_CK_TLSR8656_SS5_02_7014[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0 },
};
const FzConverter* const kFz_CK_TLSR8656_SS5_02_7014[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};
constexpr BindingSpec kBindings_CK_TLSR8656_SS5_02_7014[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
    { 1, 0x0001 },   // genPowerCfg
};
}  // namespace
extern const PreparedDefinition kDef_CK_TLSR8656_SS5_02_7014_{
    .zigbee_models=kModels_CK_TLSR8656_SS5_02_7014, .zigbee_models_count=sizeof(kModels_CK_TLSR8656_SS5_02_7014)/sizeof(kModels_CK_TLSR8656_SS5_02_7014[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CK-TLSR8656-SS5-02(7014)", .vendor="Ewelink",
    .meta=nullptr, .exposes=kExposes_CK_TLSR8656_SS5_02_7014, .exposes_count=sizeof(kExposes_CK_TLSR8656_SS5_02_7014)/sizeof(kExposes_CK_TLSR8656_SS5_02_7014[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CK_TLSR8656_SS5_02_7014, .from_zigbee_count=sizeof(kFz_CK_TLSR8656_SS5_02_7014)/sizeof(kFz_CK_TLSR8656_SS5_02_7014[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_CK_TLSR8656_SS5_02_7014, .bindings_count=sizeof(kBindings_CK_TLSR8656_SS5_02_7014)/sizeof(kBindings_CK_TLSR8656_SS5_02_7014[0]),
};
}  // namespace zhc::devices::ewelink
