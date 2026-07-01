// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Sonoff SNZB-02UL temperature & humidity sensor (z2m v26.76.0 parity).
//
// z2m-source: sonoff.ts #SNZB-02UL. Same primary decode as SNZB-02 /
// SNZB-02M (msTemperatureMeasurement 0x0402 /100, msRelativeHumidity 0x0405
// /100, genPowerCfg 0x0001). eWeLink custom cluster 0xFC11 config surface is
// manu-specific and DEFERRED.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr const char* kModels_SNZB_02UL[] = { "SNZB-02UL" };
constexpr Expose kExposes_SNZB_02UL[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV",         nullptr, nullptr, 0 },
};
const FzConverter* const kFz_SNZB_02UL[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};
constexpr BindingSpec kBindings_SNZB_02UL[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
    { 1, 0x0001 },   // genPowerCfg
};
}  // namespace
extern const PreparedDefinition kDef_SNZB_02UL{
    .zigbee_models=kModels_SNZB_02UL, .zigbee_models_count=sizeof(kModels_SNZB_02UL)/sizeof(kModels_SNZB_02UL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNZB-02UL", .vendor="Sonoff",
    .meta=nullptr, .exposes=kExposes_SNZB_02UL, .exposes_count=sizeof(kExposes_SNZB_02UL)/sizeof(kExposes_SNZB_02UL[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SNZB_02UL, .from_zigbee_count=sizeof(kFz_SNZB_02UL)/sizeof(kFz_SNZB_02UL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SNZB_02UL, .bindings_count=sizeof(kBindings_SNZB_02UL)/sizeof(kBindings_SNZB_02UL[0]),
};
}  // namespace zhc::devices::sonoff
