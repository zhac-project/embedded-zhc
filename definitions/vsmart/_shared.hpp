// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Tier 2: shared VSmart converters.
//
// VSmart wall-switches register a *manu-specific* custom cluster called
// `vsmartSwitchControl` that — unusually — claims the same wire id as
// `genBasic` (0x0000). The manufacturer code (0x1379) is what makes
// the cluster distinct on air. Every action surface (LED indicator
// colour, vibration intensity, per-period LED intensity, brightness
// presets, time periods) is exposed as a *cluster-specific command*
// with a 1-byte payload schedule — these are NOT writeAttributes and
// therefore cannot be encoded by the generic `tz_zcl_write_attr`.
//
// This header defines a tiny per-command Tz encoder
// (`tz_vsmart_raw_cmd`) plus a `VsmartCmdSpec` describing the command
// id and how the payload bytes are derived from the input Value.
//
// z2m-source: zigbee-herdsman-converters/src/devices/vsmart.ts.

#include <cstdint>
#include <string_view>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::vsmart {

// Manufacturer code (Z2M `VSMART_MANUFACTURER_CODE`).
inline constexpr std::uint16_t kManufacturerCode = 0x1379;

// Cluster id of vsmart's custom `vsmartSwitchControl` cluster.
// Yes — it shadows genBasic (0x0000) on the wire; manu code is the
// discriminator.
inline constexpr std::uint16_t kClusterId = 0x0000;

// Encoder selectors for `VsmartCmdSpec::encoder`. Each selector reads
// the input Value (already type-checked by the runtime) and builds the
// payload that follows the manu-specific ZCL header.
enum class VsmartEncoder : std::uint8_t {
    // setLedColor (cmd 0xF3): 4 bytes [R, G, B, state]
    //   `state` is 0x00 for the OFF colour, 0x01 for the ON colour
    //   (selected by the spec's `state` field). Input must be a string
    //   in `#RRGGBB` form (case-insensitive, leading `#` optional).
    LedColor = 0,

    // setVibrationIntensity (cmd 0xF0): 1 byte
    //   payload = round(percent * 255 / 100), percent ∈ [0,100].
    VibrationIntensity = 1,

    // setLedIntensity (cmd 0xF2): 3 bytes [intensity, endpoint_index, 0]
    //   intensity = round(percent * 255 / 100), percent ∈ [0,100].
    //   endpoint_index is fixed per spec (0=morning, 1=evening, 2=night).
    LedIntensity = 2,
};

struct VsmartCmdSpec {
    const char*    key;          // input payload key claimed by this Tz
    std::uint8_t   command_id;   // ZCL command id (0xF0/0xF2/0xF3/...)
    VsmartEncoder  encoder;
    // Encoder-specific extra byte (LedColor: state byte; LedIntensity:
    // endpoint index). Ignored otherwise.
    std::uint8_t   extra;
};

// One generic encoder used by every per-command TzConverter via
// `user_config = &kSpec...`.  Stamps fc=0x15 (cluster-specific, manu,
// disable-default-response), then [manu_lo][manu_hi][tsn][cmd][bytes...].
bool tz_vsmart_raw_cmd(std::string_view key,
                       const Value& input,
                       const TzConverter& self,
                       const PreparedDefinition& def,
                       RuntimeContext& ctx,
                       std::span<std::uint8_t> out_frame,
                       std::size_t& out_size);

// Per-command Tz converters. Wired into device cpps as needed.
extern const TzConverter kTzLedIndicatorColorOn;
extern const TzConverter kTzLedIndicatorColorOff;
extern const TzConverter kTzVibrationIntensity;
extern const TzConverter kTzMorningLedIntensity;
extern const TzConverter kTzEveningLedIntensity;
extern const TzConverter kTzNightLedIntensity;

}  // namespace zhc::vsmart
