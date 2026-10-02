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
//
// z2m v26.115.1 window: pressure comes from msPressureMeasurement attribute
// 0x0004 (int32, hundredths of hPa), no longer read with the eWeLink
// manufacturer code, and is reported 5 s .. 1 h on a 0.5 hPa change;
// temperature / humidity report 5 s .. 1 h on 0.2 C / 1 %.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr const char* kModels_SNZB_02M[] = { "SNZB-02M" };
constexpr Expose kExposes_SNZB_02M[] = {
    { "battery",     ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV",         nullptr, nullptr, 0 },
    { "pressure",    ExposeType::Numeric, Access::State, "hPa",        nullptr, nullptr, 0 },
};
constexpr ::zhc::generic::ZclAttrRow kPressureRows[] = { { 0x0004, "pressure", 100 } };
constexpr ::zhc::generic::ZclAttrMap kPressureMap{ kPressureRows, 1 };
constexpr FzConverter kFzPressure0004 = ::zhc::generic::zcl_attr_fz("msPressureMeasurement", &kPressureMap);
const FzConverter* const kFz_SNZB_02M[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
    &kFzPressure0004,
};
constexpr BindingSpec kBindings_SNZB_02M[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0403 },   // msPressureMeasurement
};
constexpr ReportingSpec kReports_SNZB_02M[] = {
    { 1, 0x0402, 0x0000, 0x29, 5, 3600, 20, 0 },
    { 1, 0x0405, 0x0000, 0x21, 5, 3600, 100, 0 },
    { 1, 0x0403, 0x0004, 0x2B, 5, 3600, 50, 0 },
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
    .reports=kReports_SNZB_02M, .reports_count=sizeof(kReports_SNZB_02M)/sizeof(kReports_SNZB_02M[0]),
};
}  // namespace zhc::devices::sonoff
