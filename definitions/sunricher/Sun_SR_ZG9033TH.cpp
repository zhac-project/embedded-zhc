// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sunricher SR-ZG9033TH — graduated from generated/ to fix a real parity gap.
// Zigbee temperature and humidity sensor (temperature on ep1, humidity on ep2).
// z2m-source: sunricher.ts #SR-ZG9033TH — m.temperature(), m.humidity({endpointNames:["2"]}),
//   m.battery(), + custom temperature_sensor_compensation config attr.
//
// PARITY FIX (missing core sensors): the auto-generated port lowered only
// kFzBattery, dropping the device's whole purpose — temperature and humidity. Add
// kFzTemperature + kFzHumidity (generic /100 °C and /100 %RH, matching z2m) and the
// msTemperatureMeasurement / msRelativeHumidity binds. Both converters use a
// wildcard endpoint, so the ep1 temperature and ep2 humidity reports both decode.
// The retained endpoint_map fans the keys out per the codebase's multi-endpoint
// convention (→ temperature_1 / humidity_2), consistent with z2m's per-endpoint
// humidity_2 keying for this two-endpoint device.
//
// DEFERRED (infra): temperature_sensor_compensation is a manuSpecific config attr
// (cluster 0x0402 attr 0x1000) with no generic converter.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG9033TH[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_SR_ZG9033TH[] = { "ZG9032B" };

constexpr ::zhc::EndpointLabel kEndpoints_SR_ZG9033TH[] = { {"1", 1}, {"2", 2} };

}  // namespace

constexpr Expose kExposes_SR_ZG9033TH[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_SR_ZG9033TH[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement (ep1)
    {2, 0x0405},  // msRelativeHumidity (ep2)
};

extern const PreparedDefinition kDef_SR_ZG9033TH{
    .zigbee_models=kModels_SR_ZG9033TH, .zigbee_models_count=sizeof(kModels_SR_ZG9033TH)/sizeof(kModels_SR_ZG9033TH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG9033TH", .vendor="Sunricher",
    .meta=nullptr, .exposes=kExposes_SR_ZG9033TH, .exposes_count=sizeof(kExposes_SR_ZG9033TH)/sizeof(kExposes_SR_ZG9033TH[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SR_ZG9033TH, .from_zigbee_count=sizeof(kFz_SR_ZG9033TH)/sizeof(kFz_SR_ZG9033TH[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_SR_ZG9033TH,.bindings_count=sizeof(kBindings_SR_ZG9033TH)/sizeof(kBindings_SR_ZG9033TH[0]),
    .endpoint_map       = kEndpoints_SR_ZG9033TH,
    .endpoint_map_count = sizeof(kEndpoints_SR_ZG9033TH)/sizeof(kEndpoints_SR_ZG9033TH[0]),
};

}  // namespace zhc::devices::sunricher
