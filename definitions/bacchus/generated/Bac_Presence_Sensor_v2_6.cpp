// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bacchus Presence_Sensor_v2.6 — uses lumi shared kFzOccupancy.
// 3-endpoint presence + illuminance sensor (mmWave variant with target_type).
// z2m-source: bacchus.ts #Presence_Sensor_v2.6.
//
// Same wire shape as Presence_Sensor_v2 with extra manu fields:
//   - msOccupancySensing manu attr 0xf006 enum8 (target_type:
//     none/moving/stationary/moving_and_stationary).
//   - msOccupancySensing manu attr 0xf007 (measurement_period sec).
//   - msIlluminanceMeasurement manu attr 0xf001 (illuminance_threshold).
//
// Coverage with Tier-1/2 generic + lumi converters:
//   - kFzOnOff       → genOnOff (0x0006) — presence state.
//   - kFzOccupancy   → msOccupancySensing (0x0406) — bit0 occupancy.
//   - kFzIlluminance → msIlluminanceMeasurement (0x0400) — raw lx.
//   - kFzBattery     → genPowerCfg (0x0001) — battery / voltage.
//
// Gaps (manu attrs requiring a bacchus shared helper):
//   - target_type, measurement_period, illuminance_threshold,
//     local_time, led_mode, min_time, max_time, day_output, night_output.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {

const FzConverter* const kFz_Presence_Sensor_v2_6[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_Presence_Sensor_v2_6[] = { "Presence_Sensor_v2.6" };

constexpr const char* kEnumTargetType[] = { "none", "moving", "stationary", "moving_and_stationary" };

constexpr Expose kExp_Presence_Sensor_v2_6[] = {
    { "occupancy",              ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "illuminance",            ExposeType::Numeric, ::zhc::Access::State,    "lx",    nullptr, nullptr, 0 },
    { "state",                  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "target_type",            ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Target type",
      kEnumTargetType, sizeof(kEnumTargetType)/sizeof(kEnumTargetType[0]) },
    { "measurement_period",     ExposeType::Numeric, ::zhc::Access::StateSet, "sec",   "Occupancy measurement period", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "battery",                ExposeType::Numeric, ::zhc::Access::State,    "%",     nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",                ExposeType::Numeric, ::zhc::Access::State,    "mV",    nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "local_time",             ExposeType::String,  ::zhc::Access::State,    nullptr, "Local time (HH:MM)", nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "led_mode",               ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Led working mode",   nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "illuminance_threshold",  ExposeType::Numeric, ::zhc::Access::StateSet, "raw",   "Illuminance threshold", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "min_time",               ExposeType::String,  ::zhc::Access::StateSet, nullptr, "Day start (HH:MM)",  nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "max_time",               ExposeType::String,  ::zhc::Access::StateSet, nullptr, "Day end (HH:MM)",    nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "day_output",             ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Output during the day",   nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "night_output",           ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Output during the night", nullptr, 0,
      ::zhc::ExposeCategory::Config },
};

constexpr BindingSpec kBind_Presence_Sensor_v2_6[] = {
    { 1, 0x0001 },
    { 1, 0x0006 },
    { 1, 0x0406 },
    { 1, 0x0400 },
    { 3, 0x0006 },
};

constexpr ::zhc::EndpointLabel kEndpoints_Presence_Sensor_v2_6[] = { {"1", 1} };

}  // namespace

extern const PreparedDefinition kDef_Presence_Sensor_v2_6{
    .zigbee_models=kModels_Presence_Sensor_v2_6, .zigbee_models_count=sizeof(kModels_Presence_Sensor_v2_6)/sizeof(kModels_Presence_Sensor_v2_6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Presence_Sensor_v2.6", .vendor="Bacchus",
    .meta=nullptr, .exposes=kExp_Presence_Sensor_v2_6, .exposes_count=sizeof(kExp_Presence_Sensor_v2_6)/sizeof(kExp_Presence_Sensor_v2_6[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Presence_Sensor_v2_6, .from_zigbee_count=sizeof(kFz_Presence_Sensor_v2_6)/sizeof(kFz_Presence_Sensor_v2_6[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Presence_Sensor_v2_6, .bindings_count=sizeof(kBind_Presence_Sensor_v2_6)/sizeof(kBind_Presence_Sensor_v2_6[0]),
    .endpoint_map       = kEndpoints_Presence_Sensor_v2_6,
    .endpoint_map_count = sizeof(kEndpoints_Presence_Sensor_v2_6)/sizeof(kEndpoints_Presence_Sensor_v2_6[0]),
};

}  // namespace zhc::devices::bacchus
