// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Inovelli writeable manuSpec parameter specs. Mirrors the
// CTM / Danfoss pattern: each attribute gets a `ZclWriteSpec` + a
// `TzConverter` that funnels through the generic `tz_zcl_write_attr`
// (foundation writeAttributes encoder, fc=0x14 with mfg=0x122f).
//
// z2m-source: zigbee-herdsman-converters/src/lib/inovelli.ts —
//             COMMON_ATTRIBUTES + COMMON_DIMMER_ATTRIBUTES.
#include "definitions/inovelli/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

namespace zhc::devices::inovelli {

namespace {

// ── ZCL data-type bytes (per ZCL spec / zigbee-herdsman DataType) ──
constexpr std::uint8_t kZclBool   = 0x10;
constexpr std::uint8_t kZclUint8  = 0x20;
constexpr std::uint8_t kZclUint16 = 0x21;

// Cluster name string used by `tz_zcl_write_attr` to match the
// runtime context cluster. z2m uses
// `manuSpecificInovelli` for cluster 0xFC31. The runtime maps that
// name back to 0xFC31 only if the foundation parser knows it; for a
// pure write path the encoder cares about cluster_id.
constexpr const char* kClusterName = "manuSpecificInovelli";

// COMMON_ATTRIBUTES (cluster 0xFC31, mfg 0x122f) —————————————————
constexpr ::zhc::generic::ZclWriteSpec kSpecDimSpeedUpRemote{
    "dimmingSpeedUpRemote",   1, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDimSpeedUpLocal{
    "dimmingSpeedUpLocal",    2, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRampOffToOnRemote{
    "rampRateOffToOnRemote",  3, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRampOffToOnLocal{
    "rampRateOffToOnLocal",   4, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDimSpeedDownRemote{
    "dimmingSpeedDownRemote", 5, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDimSpeedDownLocal{
    "dimmingSpeedDownLocal",  6, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRampOnToOffRemote{
    "rampRateOnToOffRemote",  7, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRampOnToOffLocal{
    "rampRateOnToOffLocal",   8, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
// invertSwitch — z2m exposes as enum {No:0, Yes:1}. Accept bool too;
// `tz_zcl_write_attr` Bool→Uint coercion handles it.
constexpr ::zhc::generic::ZclWriteSpec kSpecInvertSwitch{
    "invertSwitch",          11, kZclBool,  kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecAutoTimerOff{
    "autoTimerOff",          12, kZclUint16, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDefaultLevelLocal{
    "defaultLevelLocal",     13, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDefaultLevelRemote{
    "defaultLevelRemote",    14, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecStateAfterPowerRestored{
    "stateAfterPowerRestored", 15, kZclUint8, kInovelliMfgCode, nullptr, 0,
};

// COMMON_DIMMER_ATTRIBUTES (cluster 0xFC31, mfg 0x122f) ———————————
constexpr ::zhc::generic::ZclWriteSpec kSpecMinimumLevel{
    "minimumLevel",           9, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMaximumLevel{
    "maximumLevel",          10, kZclUint8, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecOutputMode{
    "outputMode",            21, kZclBool, kInovelliMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecSmartBulbMode{
    "smartBulbMode",         22, kZclBool, kInovelliMfgCode, nullptr, 0,
};

}  // namespace

#define ZHC_INO_TZ(var, spec_ref, key_str)                           \
    extern const TzConverter var{                                    \
        .key         = key_str,                                      \
        .cluster     = kClusterName,                                 \
        .cluster_id  = kInovelliClusterId,                           \
        .command_id  = 0x02,                                         \
        .fn          = &::zhc::generic::tz_zcl_write_attr,           \
        .user_config = &spec_ref,                                    \
    };

ZHC_INO_TZ(kTzInoDimmingSpeedUpRemote,    kSpecDimSpeedUpRemote,
           "dimmingSpeedUpRemote")
ZHC_INO_TZ(kTzInoDimmingSpeedUpLocal,     kSpecDimSpeedUpLocal,
           "dimmingSpeedUpLocal")
ZHC_INO_TZ(kTzInoRampRateOffToOnRemote,   kSpecRampOffToOnRemote,
           "rampRateOffToOnRemote")
ZHC_INO_TZ(kTzInoRampRateOffToOnLocal,    kSpecRampOffToOnLocal,
           "rampRateOffToOnLocal")
ZHC_INO_TZ(kTzInoDimmingSpeedDownRemote,  kSpecDimSpeedDownRemote,
           "dimmingSpeedDownRemote")
ZHC_INO_TZ(kTzInoDimmingSpeedDownLocal,   kSpecDimSpeedDownLocal,
           "dimmingSpeedDownLocal")
ZHC_INO_TZ(kTzInoRampRateOnToOffRemote,   kSpecRampOnToOffRemote,
           "rampRateOnToOffRemote")
ZHC_INO_TZ(kTzInoRampRateOnToOffLocal,    kSpecRampOnToOffLocal,
           "rampRateOnToOffLocal")
ZHC_INO_TZ(kTzInoInvertSwitch,            kSpecInvertSwitch,
           "invertSwitch")
ZHC_INO_TZ(kTzInoAutoTimerOff,            kSpecAutoTimerOff,
           "autoTimerOff")
ZHC_INO_TZ(kTzInoDefaultLevelLocal,       kSpecDefaultLevelLocal,
           "defaultLevelLocal")
ZHC_INO_TZ(kTzInoDefaultLevelRemote,      kSpecDefaultLevelRemote,
           "defaultLevelRemote")
ZHC_INO_TZ(kTzInoStateAfterPowerRestored, kSpecStateAfterPowerRestored,
           "stateAfterPowerRestored")
ZHC_INO_TZ(kTzInoMinimumLevel,            kSpecMinimumLevel,
           "minimumLevel")
ZHC_INO_TZ(kTzInoMaximumLevel,            kSpecMaximumLevel,
           "maximumLevel")
ZHC_INO_TZ(kTzInoOutputMode,              kSpecOutputMode,
           "outputMode")
ZHC_INO_TZ(kTzInoSmartBulbMode,           kSpecSmartBulbMode,
           "smartBulbMode")

#undef ZHC_INO_TZ

}  // namespace zhc::devices::inovelli
