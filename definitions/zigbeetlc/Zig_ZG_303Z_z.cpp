// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared zigbeetlc converters.
// HOBEIAN ZG-303Z soil sensor on pvvx ZigbeeTLc firmware (z2m v26.115.1 window).
// Endpoint 1 reports air temperature + humidity; endpoint 2 reports soil
// moisture on the relative-humidity cluster (z2m fzConvert by endpoint).
// z2m-source: zigbeetlc.ts #ZG-303Z-z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/zigbeetlc/_shared.hpp"

namespace zhc::devices::zigbeetlc {
namespace {
constexpr FzConverter on_endpoint(FzConverter c, std::uint8_t ep) { c.endpoint = ep; return c; }
constexpr ::zhc::generic::ZclAttrRow kAirRows[] = { { 0x0000, "humidity", 100 } };
constexpr ::zhc::generic::ZclAttrMap kAirMap{ kAirRows, 1 };
constexpr ::zhc::generic::ZclAttrRow kSoilRows[] = { { 0x0000, "soil_moisture", 100 } };
constexpr ::zhc::generic::ZclAttrMap kSoilMap{ kSoilRows, 1 };
constexpr FzConverter kFzAirHumidity = on_endpoint(::zhc::generic::zcl_attr_fz("msRelativeHumidity", &kAirMap), 1);
constexpr FzConverter kFzSoilMoisture = on_endpoint(::zhc::generic::zcl_attr_fz("msRelativeHumidity", &kSoilMap), 2);
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &kFzAirHumidity,
    &kFzSoilMoisture,
};
const TzConverter* const kTz[] = {
    &::zhc::zigbeetlc::kTzTemperatureCalibration,
    &::zhc::zigbeetlc::kTzHumidityCalibration,
    &::zhc::zigbeetlc::kTzMeasurementInterval,
};
constexpr Expose kExp[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, -50, 50, 0},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::Config, -50, 50, 0},
    {"measurement_interval", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::Config, 3, 255, 1},
};
constexpr const char* kM[] = { "ZG-303Z-z" };
constexpr BindingSpec kBind[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
    {2, 0x0405},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0001, 0x0021, 0x20, 3600, 65000, 10, 0},
    {1, 0x0402, 0x0000, 0x29, 10, 3600, 10, 0},
    {1, 0x0405, 0x0000, 0x21, 10, 3600, 100, 0},
    {2, 0x0405, 0x0000, 0x21, 10, 3600, 100, 0},
};
}  // namespace

extern const PreparedDefinition kDef_ZG_303Z_z{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-303Z-z", .vendor="HOBEIAN",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::zigbeetlc
