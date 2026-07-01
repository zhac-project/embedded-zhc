// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Sonoff SNZB-02M temperature & humidity sensor (z2m v26.76.0 parity).
//
// z2m-source: sonoff.ts #SNZB-02M. Primary function = temperature + humidity
// + battery, identical decode to SNZB-02 (msTemperatureMeasurement 0x0402
// /100, msRelativeHumidity 0x0405 /100, genPowerCfg 0x0001). The eWeLink
// custom cluster 0xFC11 config surface (temperature/humidity calibration,
// comfort thresholds) is manu-specific and DEFERRED — mirrors the
// "port the standard stream, defer vendor config" precedent.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr const char* kModels_SNZB_02M[] = { "SNZB-02M" };
constexpr Expose kExposes_SNZB_02M[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV",         nullptr, nullptr, 0 },
};
const FzConverter* const kFz_SNZB_02M[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};
constexpr BindingSpec kBindings_SNZB_02M[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
    { 1, 0x0001 },   // genPowerCfg
};
}  // namespace
extern const PreparedDefinition kDef_SNZB_02M{
    .zigbee_models=kModels_SNZB_02M, .zigbee_models_count=sizeof(kModels_SNZB_02M)/sizeof(kModels_SNZB_02M[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNZB-02M", .vendor="Sonoff",
    .meta=nullptr, .exposes=kExposes_SNZB_02M, .exposes_count=sizeof(kExposes_SNZB_02M)/sizeof(kExposes_SNZB_02M[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SNZB_02M, .from_zigbee_count=sizeof(kFz_SNZB_02M)/sizeof(kFz_SNZB_02M[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SNZB_02M, .bindings_count=sizeof(kBindings_SNZB_02M)/sizeof(kBindings_SNZB_02M[0]),
};
}  // namespace zhc::devices::sonoff
