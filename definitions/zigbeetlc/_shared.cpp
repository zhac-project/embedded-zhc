// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared ZigbeeTLc converter implementations. See _shared.hpp for the
// attribute table.
//
// z2m-source: zigbee-herdsman-converters/src/devices/zigbeetlc.ts
//             (`const extend = { … }` block at top of file).

#include "definitions/zigbeetlc/_shared.hpp"

#include <cstdint>

namespace zhc::zigbeetlc {

namespace {

// ZCL data type constants used below:
//   ENUM8   = 0x30
//   UINT8   = 0x20
//   UINT16  = 0x21
//   INT16   = 0x29

constexpr ::zhc::generic::ZclWriteLookup kTempDisplayModeLut[] = {
    {"celsius",    0},
    {"fahrenheit", 1},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecTempDisplayMode{
    "temperature_display_mode", 0x0000, 0x30, 0,
    kTempDisplayModeLut, sizeof(kTempDisplayModeLut)/sizeof(kTempDisplayModeLut[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecComfortSmiley{
    "comfort_smiley",           0x0002, 0x30, 0, nullptr, 0,
    ::zhc::generic::kZclWriteFlagInvertBool,  // valueOn=[true,0]
};
constexpr ::zhc::generic::ZclWriteSpec kSpecTemperatureCalibration{
    "temperature_calibration",  0x0100, 0x29, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecHumidityCalibration{
    "humidity_calibration",     0x0101, 0x29, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecComfortTemperatureMin{
    "comfort_temperature_min",  0x0102, 0x29, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecComfortTemperatureMax{
    "comfort_temperature_max",  0x0103, 0x29, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecComfortHumidityMin{
    "comfort_humidity_min",     0x0104, 0x21, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecComfortHumidityMax{
    "comfort_humidity_max",     0x0105, 0x21, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecEnableDisplay{
    "enable_display",           0x0106, 0x30, 0, nullptr, 0,
    ::zhc::generic::kZclWriteFlagInvertBool,  // valueOn=[true,0]
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMeasurementInterval{
    "measurement_interval",     0x0107, 0x20, 0, nullptr, 0,
};

}  // namespace

#define ZHC_ZTLC_TZ(var, spec_ref, key_str)                              \
    extern const TzConverter var{                                        \
        .key         = key_str,                                          \
        .cluster     = "hvacUserInterfaceCfg",                           \
        .cluster_id  = 0x0204,                                           \
        .command_id  = 0x02,    /* writeAttributes */                    \
        .fn          = &::zhc::generic::tz_zcl_write_attr,               \
        .user_config = &spec_ref,                                        \
    };

ZHC_ZTLC_TZ(kTzTempDisplayMode,        kSpecTempDisplayMode,        "temperature_display_mode")
ZHC_ZTLC_TZ(kTzComfortSmiley,          kSpecComfortSmiley,          "comfort_smiley")
ZHC_ZTLC_TZ(kTzTemperatureCalibration, kSpecTemperatureCalibration, "temperature_calibration")
ZHC_ZTLC_TZ(kTzHumidityCalibration,    kSpecHumidityCalibration,    "humidity_calibration")
ZHC_ZTLC_TZ(kTzComfortTemperatureMin,  kSpecComfortTemperatureMin,  "comfort_temperature_min")
ZHC_ZTLC_TZ(kTzComfortTemperatureMax,  kSpecComfortTemperatureMax,  "comfort_temperature_max")
ZHC_ZTLC_TZ(kTzComfortHumidityMin,     kSpecComfortHumidityMin,     "comfort_humidity_min")
ZHC_ZTLC_TZ(kTzComfortHumidityMax,     kSpecComfortHumidityMax,     "comfort_humidity_max")
ZHC_ZTLC_TZ(kTzEnableDisplay,          kSpecEnableDisplay,          "enable_display")
ZHC_ZTLC_TZ(kTzMeasurementInterval,    kSpecMeasurementInterval,    "measurement_interval")

// ── Shared exposes (identical for every zigbeetlc device) ─────────
extern const ::zhc::Expose kSharedExposes[] = {
    {"battery",                  ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",                  ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0},
    {"temperature",              ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "C",  nullptr, nullptr, 0},
    {"humidity",                 ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0},
    {"enable_display",           ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"temperature_display_mode", ::zhc::ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"comfort_smiley",           ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"comfort_temperature_min",  ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"comfort_temperature_max",  ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"comfort_humidity_min",     ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "%",  nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"comfort_humidity_max",     ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "%",  nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"temperature_calibration",  ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "C",  nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"humidity_calibration",     ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "%",  nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
    {"measurement_interval",     ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "s",  nullptr, nullptr, 0, ::zhc::ExposeCategory::Config},
};
extern const std::uint8_t kSharedExposesCount =
    sizeof(kSharedExposes) / sizeof(kSharedExposes[0]);

// ── Shared from-zigbee table ──────────────────────────────────────
extern const ::zhc::FzConverter* const kSharedFz[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
extern const std::uint8_t kSharedFzCount =
    sizeof(kSharedFz) / sizeof(kSharedFz[0]);

// ── Shared to-zigbee table ────────────────────────────────────────
extern const ::zhc::TzConverter* const kSharedTz[] = {
    &kTzEnableDisplay,
    &kTzTempDisplayMode,
    &kTzComfortSmiley,
    &kTzComfortTemperatureMin,
    &kTzComfortTemperatureMax,
    &kTzComfortHumidityMin,
    &kTzComfortHumidityMax,
    &kTzTemperatureCalibration,
    &kTzHumidityCalibration,
    &kTzMeasurementInterval,
};
extern const std::uint8_t kSharedTzCount =
    sizeof(kSharedTz) / sizeof(kSharedTz[0]);

// ── Shared bindings ───────────────────────────────────────────────
extern const ::zhc::BindingSpec kSharedBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0204},  // hvacUserInterfaceCfg
};
extern const std::uint8_t kSharedBindingsCount =
    sizeof(kSharedBindings) / sizeof(kSharedBindings[0]);

}  // namespace zhc::zigbeetlc
