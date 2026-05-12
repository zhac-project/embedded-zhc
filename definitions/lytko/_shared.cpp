// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Lytko shared write-attribute encoders. Wraps the generic
// `tz_zcl_write_attr` engine (definitions/_generic/_shared.{hpp,cpp})
// with per-attribute `ZclWriteSpec`s — one TzConverter per attribute
// keeps the per-device wiring trivial:
//
//     const TzConverter* const kTz_X[] = {
//         &::zhc::generic::kTzThermostat,
//         &::zhc::devices::lytko::kTzLytkoSensorType,
//         &::zhc::devices::lytko::kTzLytkoOccupiedSetback,
//         ...
//     };
//
// z2m-source: zigbee-herdsman-converters/src/devices/lytko.ts.

#include "definitions/lytko/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lytko {

namespace {

// ZCL data type bytes (zigbee-herdsman zspec).
constexpr std::uint8_t kZclTypeBool = 0x10;
constexpr std::uint8_t kZclTypeUint8 = 0x20;
constexpr std::uint8_t kZclTypeEnum8 = 0x30;

// Lytko's manu code, 0x7777 (Zcl.ManufacturerCode.CUSTOM_LYTKO).
constexpr std::uint16_t kManuLytko = 0x7777;

// Lookup for "sensor_type" string → enum8 raw — the index into
// the lytko.ts sensorTypes array. Drivers send the raw u8 directly
// when the payload is numeric, so the lookup is best-effort.
constexpr ::zhc::generic::ZclWriteLookup kSensorTypeLookup[] = {
    { "3.3",  0 }, { "5",   1 }, { "6.8", 2 }, { "10",  3 },
    { "12",   4 }, { "14.8",5 }, { "15",  6 }, { "20",  7 },
    { "33",   8 }, { "47",  9 },
};
constexpr std::size_t kSensorTypeLookupCount =
    sizeof(kSensorTypeLookup) / sizeof(kSensorTypeLookup[0]);

// hvacThermostat (0x0201) writes.
constexpr ::zhc::generic::ZclWriteSpec kSpecSensorType{
    "sensor_type", 0x7700, kZclTypeEnum8, kManuLytko,
    kSensorTypeLookup, kSensorTypeLookupCount,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecTargetTempFirst{
    "target_temp_first", 0x7701, kZclTypeBool, kManuLytko, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecOccupiedSetback{
    "occupied_setback", 0x0034, kZclTypeUint8, 0, nullptr, 0,
};

// hvacUserInterfaceCfg (0x0204) writes.
constexpr ::zhc::generic::ZclWriteSpec kSpecKeypadLockout{
    "keypad_lockout", 0x0001, kZclTypeEnum8, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecBrightnessActive{
    "brightness", 0x7700, kZclTypeUint8, kManuLytko, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecBrightnessStandby{
    "brightness_standby", 0x7701, kZclTypeUint8, kManuLytko, nullptr, 0,
};

}  // namespace

// Macro stamps out a `TzConverter` keyed by `key_str`, dispatched on
// (cluster, cluster_id) with cmd=0x02 (writeAttributes). Mirrors the
// `ZHC_LUMI_TZ` shape in `definitions/lumi/_shared.cpp`.
#define ZHC_LYTKO_TZ(var, spec_ref, key_str, cluster_str, cluster_id_v) \
    extern const TzConverter var{                                       \
        .key         = key_str,                                         \
        .cluster     = cluster_str,                                     \
        .cluster_id  = cluster_id_v,                                    \
        .command_id  = 0x02,                                            \
        .fn          = &::zhc::generic::tz_zcl_write_attr,              \
        .user_config = &spec_ref,                                       \
    };

ZHC_LYTKO_TZ(kTzLytkoSensorType,        kSpecSensorType,
             "sensor_type",        "hvacThermostat",       0x0201)
ZHC_LYTKO_TZ(kTzLytkoTargetTempFirst,   kSpecTargetTempFirst,
             "target_temp_first",  "hvacThermostat",       0x0201)
ZHC_LYTKO_TZ(kTzLytkoOccupiedSetback,   kSpecOccupiedSetback,
             "occupied_setback",   "hvacThermostat",       0x0201)
ZHC_LYTKO_TZ(kTzLytkoKeypadLockout,     kSpecKeypadLockout,
             "keypad_lockout",     "hvacUserInterfaceCfg", 0x0204)
ZHC_LYTKO_TZ(kTzLytkoBrightnessActive,  kSpecBrightnessActive,
             "brightness",         "hvacUserInterfaceCfg", 0x0204)
ZHC_LYTKO_TZ(kTzLytkoBrightnessStandby, kSpecBrightnessStandby,
             "brightness_standby", "hvacUserInterfaceCfg", 0x0204)

#undef ZHC_LYTKO_TZ

}  // namespace zhc::devices::lytko
