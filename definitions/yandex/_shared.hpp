// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared Yandex (Alice ecosystem) converters.
//
// Yandex devices ship a single manufacturer-specific cluster
// `manuSpecificYandex` (cluster 0xFC03) used as the config side-channel
// for every wired switch / dimmer / relay. Two manufacturer codes are
// in flight: 0x140A ("Old", switches & dimmers) and 0x132F ("New",
// thermostatic radiator valve, window cover).
//
// z2m-source: zigbee-herdsman-converters/src/devices/yandex.ts

#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::yandex {

constexpr std::uint16_t kClusterYandex = 0xFC03;
constexpr std::uint16_t kManuOld       = 0x140A;
constexpr std::uint16_t kManuNew       = 0x132F;

// Pre-declared TzConverters for every writable manuSpecificYandex
// attribute. All use `tz_zcl_write_attr` with the matching ZclWriteSpec
// in the implementation TU; per-device ports just reference these
// pointers from their kTz_<MODEL>[] arrays.
extern const ::zhc::TzConverter kTzYxSwitchMode;       // operation_mode (ENUM8)
extern const ::zhc::TzConverter kTzYxSwitchType;       // switch_type    (ENUM8)
extern const ::zhc::TzConverter kTzYxPowerType;        // power_type     (ENUM8)
extern const ::zhc::TzConverter kTzYxLedIndicator;     // led_indicator  (BOOL)
extern const ::zhc::TzConverter kTzYxInterlock;        // interlock      (BOOL)
extern const ::zhc::TzConverter kTzYxButtonMode;       // button_mode    (ENUM8)
extern const ::zhc::TzConverter kTzYxDisplayFlip;      // display_flip   (BOOL)
extern const ::zhc::TzConverter kTzYxWindowDetection;  // window_detection (BOOL)
extern const ::zhc::TzConverter kTzYxFrostProtection;  // frost_protection (BOOL)
extern const ::zhc::TzConverter kTzYxScaleProtection;  // scale_protection (BOOL)
extern const ::zhc::TzConverter kTzYxAutoCalibration;  // auto_calibration (BOOL)

}  // namespace zhc::yandex
