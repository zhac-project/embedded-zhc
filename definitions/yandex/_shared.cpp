// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared Yandex (Alice ecosystem) converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/yandex.ts

#include "definitions/yandex/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

namespace zhc::yandex {

namespace {

// ZCL data-type bytes used by Yandex's mfg-specific attributes.
constexpr std::uint8_t kZclEnum8 = 0x30;
constexpr std::uint8_t kZclBool  = 0x10;

// One ZclWriteSpec per writable manuSpecificYandex attribute. The
// macros below wrap them into TzConverters that run on cluster 0xFC03
// (manuSpecificYandex) using `writeAttribute` (cmd 0x02) with the
// "Old" manufacturer code 0x140A — that's what every switch / dimmer
// in z2m emits. The "New" 0x132F code is only used by the TRV (518)
// and cover (591); per-device ports for those declare their own
// ZclWriteSpec inline (see e.g. Yan_YNDX_00591.cpp).

constexpr ::zhc::generic::ZclWriteSpec kSpecSwitchMode      { "operation_mode",  0x0001, kZclEnum8, kManuOld, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecSwitchType      { "switch_type",     0x0002, kZclEnum8, kManuOld, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecPowerType       { "power_type",      0x0003, kZclEnum8, kManuOld, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecLedIndicator    { "led_indicator",   0x0005, kZclBool,  kManuOld, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecInterlock       { "interlock",       0x0007, kZclBool,  kManuOld, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecButtonMode      { "button_mode",     0x0008, kZclEnum8, kManuOld, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecDisplayFlip     { "display_flip",    0x0009, kZclBool,  kManuNew, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecWindowDetection { "window_detection",0x000A, kZclBool,  kManuNew, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecFrostProtection { "frost_protection",0x000D, kZclBool,  kManuNew, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecScaleProtection { "scale_protection",0x000E, kZclBool,  kManuNew, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecAutoCalibration { "auto_calibration",0x000F, kZclBool,  kManuNew, nullptr, 0 };

}  // namespace

#define ZHC_YANDEX_TZ(var, spec_ref, key_str)                                  \
    extern const ::zhc::TzConverter var{                                       \
        .key         = key_str,                                                \
        .cluster     = "manuSpecificYandex",                                   \
        .cluster_id  = kClusterYandex,                                         \
        .command_id  = 0x02,  /* writeAttribute */                             \
        .fn          = &::zhc::generic::tz_zcl_write_attr,                     \
        .user_config = &spec_ref,                                              \
    };

ZHC_YANDEX_TZ(kTzYxSwitchMode,      kSpecSwitchMode,      "operation_mode")
ZHC_YANDEX_TZ(kTzYxSwitchType,      kSpecSwitchType,      "switch_type")
ZHC_YANDEX_TZ(kTzYxPowerType,       kSpecPowerType,       "power_type")
ZHC_YANDEX_TZ(kTzYxLedIndicator,    kSpecLedIndicator,    "led_indicator")
ZHC_YANDEX_TZ(kTzYxInterlock,       kSpecInterlock,       "interlock")
ZHC_YANDEX_TZ(kTzYxButtonMode,      kSpecButtonMode,      "button_mode")
ZHC_YANDEX_TZ(kTzYxDisplayFlip,     kSpecDisplayFlip,     "display_flip")
ZHC_YANDEX_TZ(kTzYxWindowDetection, kSpecWindowDetection, "window_detection")
ZHC_YANDEX_TZ(kTzYxFrostProtection, kSpecFrostProtection, "frost_protection")
ZHC_YANDEX_TZ(kTzYxScaleProtection, kSpecScaleProtection, "scale_protection")
ZHC_YANDEX_TZ(kTzYxAutoCalibration, kSpecAutoCalibration, "auto_calibration")

#undef ZHC_YANDEX_TZ

}  // namespace zhc::yandex
