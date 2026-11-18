// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0201 — temperature + humidity + battery sensor.
// z2m-source: tuya.ts #TS0201.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

const FzConverter* const kFz_TS0201[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_TS0201[] = { "TS0201", "SNTZ003", "TY0201" };

// fzLocal.TS0201_humidity: these five report measuredValue /10 too low, z2m multiplies by 10 (so /10 here).
constexpr ::zhc::generic::ZclAttrRow kHumidity10Rows[] = { { 0x0000, "humidity", 10 } };
constexpr ::zhc::generic::ZclAttrMap kHumidity10Map{ kHumidity10Rows, 1, 0 };
constexpr FzConverter kFzHumidity10 = ::zhc::generic::zcl_attr_fz("msRelativeHumidity", &kHumidity10Map);
const FzConverter* const kFz_TS0201_humidity10[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &kFzHumidity10,
};
constexpr const char* kModels_TS0201_humidity10[] = { "TS0201" };
constexpr const char* kManus_TS0201_humidity10[] = {
    "_TZ3210_ncw88jfq", "_TZ3000_ywagc4rj", "_TZ3000_isw9u95y", "_TZ3000_yupc0pb7", "_TZ3000_wol32olt" };

constexpr Expose kExposes_TS0201[] = {
    { "temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_TS0201[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
};
// UZ-05T reports manufacturerName "Wing" (z2m v26.133.0, #13838); the bare zigbeeModel
// match covers it, the white label only names it.
constexpr WhiteLabel kWhiteLabels_TS0201[] = {
    {"Shenzhen Forever Young Technology", "UZ-05T"},
};

}  // namespace

extern const PreparedDefinition kDef_TS0201{
    .zigbee_models            = kModels_TS0201,
    .zigbee_models_count      = sizeof(kModels_TS0201)/sizeof(kModels_TS0201[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "TS0201",
    .vendor                   = "Tuya",
    .meta                     = nullptr,
    .exposes                  = kExposes_TS0201,
    .exposes_count            = sizeof(kExposes_TS0201)/sizeof(kExposes_TS0201[0]),
    .white_labels             = kWhiteLabels_TS0201,
    .white_labels_count       = sizeof(kWhiteLabels_TS0201)/sizeof(kWhiteLabels_TS0201[0]),
    .from_zigbee              = kFz_TS0201,
    .from_zigbee_count        = sizeof(kFz_TS0201)/sizeof(kFz_TS0201[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_TS0201,
    .bindings_count           = sizeof(kBindings_TS0201)/sizeof(kBindings_TS0201[0]),
};

// Same sensor, the five TS0201_humidity manus (z2m v26.133.0 tuya.ts fzLocal.TS0201_humidity).
extern const PreparedDefinition kDef_TS0201_humidity10{
    .zigbee_models            = kModels_TS0201_humidity10,
    .zigbee_models_count      = 1,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = kManus_TS0201_humidity10,
    .manufacturer_names_count = sizeof(kManus_TS0201_humidity10)/sizeof(kManus_TS0201_humidity10[0]),
    .model                    = "TS0201",
    .vendor                   = "Tuya",
    .meta                     = nullptr,
    .exposes                  = kExposes_TS0201,
    .exposes_count            = sizeof(kExposes_TS0201)/sizeof(kExposes_TS0201[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_TS0201_humidity10,
    .from_zigbee_count        = sizeof(kFz_TS0201_humidity10)/sizeof(kFz_TS0201_humidity10[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_TS0201,
    .bindings_count           = sizeof(kBindings_TS0201)/sizeof(kBindings_TS0201[0]),
};

}  // namespace zhc::devices::tuya
