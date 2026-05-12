// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// ── Lumi factories ──────────────────────────────────────────────────
//
// C++ equivalents of z2m's `lumiLight(...)`, `lumiOnOff(...)`,
// `lumiAction(...)`, `lumiPower(...)`. Each factory takes a literal
// options struct as a non-type template parameter and exposes static
// constexpr arrays of converter pointers sized to match the options.
//
// Factories compose canned fz/tz converter pointers from
// `definitions/_generic/_shared.hpp` + `definitions/lumi/_shared.hpp`
// — they don't define new converters, they bundle existing ones. This
// keeps the factory layer trivially auditable: a device's
// `PreparedDefinition` reads:
//
//   using FX = factory::LumiLight<factory::LumiLightOpts{
//       .level = true, .color_temp = true,
//   }>;
//
// The factory's `fz_list` / `tz_list` arrays are then plugged into the
// PreparedDefinition by the device cpp.
//
// z2m-source: zigbee-herdsman-converters/src/lib/lumi.ts (lumiLight,
//             lumiOnOff, lumiAction, lumiPower).

#include <array>
#include <cstddef>
#include <cstdint>

#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::lumi::factory {

// ── LumiLight ─────────────────────────────────────────────────────
//
// Builds the converter bundle for Aqara lights. Covers the subset of
// z2m options actually exercised across the 22 devices that use
// `lumiLight({...})` — combinations for richer modes (color_xy, effect)
// land as they're needed.

struct LumiLightOpts {
    bool level               = false;
    bool color_temp          = false;
    bool power_outage_memory = false;  // reserved: wires configure step
                                        // in a later task.
};

namespace detail {

consteval std::size_t light_fz_count(LumiLightOpts o) {
    std::size_t n = 2;  // lumi_basic + on_off
    if (o.level)      ++n;
    if (o.color_temp) ++n;
    return n;
}

consteval std::size_t light_tz_count(LumiLightOpts o) {
    std::size_t n = 1;  // on_off
    if (o.level)      ++n;
    if (o.color_temp) ++n;
    return n;
}

template <LumiLightOpts OPTS>
struct LightFzList {
    static constexpr std::size_t N = light_fz_count(OPTS);
    static constexpr std::array<const FzConverter*, N> value = [] {
        std::array<const FzConverter*, N> arr{};
        std::size_t i = 0;
        arr[i++] = &::zhc::lumi::kFzLumiBasic;
        arr[i++] = &::zhc::generic::kFzOnOff;
        if constexpr (OPTS.level)      arr[i++] = &::zhc::generic::kFzBrightness;
        if constexpr (OPTS.color_temp) arr[i++] = &::zhc::generic::kFzColorTemperature;
        return arr;
    }();
};

template <LumiLightOpts OPTS>
struct LightTzList {
    static constexpr std::size_t N = light_tz_count(OPTS);
    static constexpr std::array<const TzConverter*, N> value = [] {
        std::array<const TzConverter*, N> arr{};
        std::size_t i = 0;
        arr[i++] = &::zhc::generic::kTzOnOff;
        if constexpr (OPTS.level)      arr[i++] = &::zhc::generic::kTzBrightness;
        if constexpr (OPTS.color_temp) arr[i++] = &::zhc::generic::kTzColorTemp;
        return arr;
    }();
};

}  // namespace detail

template <LumiLightOpts OPTS>
struct LumiLight {
    static constexpr auto fz_array = detail::LightFzList<OPTS>::value;
    static constexpr auto tz_array = detail::LightTzList<OPTS>::value;

    // Pointer-list views the device plugs directly into
    // `PreparedDefinition::from_zigbee` / `to_zigbee`. Data lives in
    // the template instantiation's `.rodata`.
    static constexpr const FzConverter* const* fz_list =
        fz_array.data();
    static constexpr std::uint8_t fz_count =
        static_cast<std::uint8_t>(fz_array.size());

    static constexpr const TzConverter* const* tz_list =
        tz_array.data();
    static constexpr std::uint8_t tz_count =
        static_cast<std::uint8_t>(tz_array.size());
};

// ── LumiOnOff ─────────────────────────────────────────────────────
//
// For Aqara wall switches and plug relays. Wraps:
//   fz:  kFzLumiBasic + device-specific `fz_lumi_on_off` bound to the
//        device's endpoint label map.
//   tz:  kTzOnOff (single-endpoint key "state"; multi-endpoint devices
//        route by endpoint at the adapter layer).
//
// The device supplies an `EndpointsT` type with:
//   static constexpr DeviceEndpointLabels map;
// When `map.count == 0` the factory degrades to the single-endpoint
// case and emits plain "state".

template <typename EndpointsT>
struct LumiOnOff {
    static constexpr FzConverter kFzOnOffBound{
        .family            = FrameFamily::Zcl,
        .cluster           = "genOnOff",
        .type_mask         = type_bit(MessageType::AttributeReport) |
                             type_bit(MessageType::ReadResponse),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &::zhc::lumi::fz_lumi_on_off },
        .user_config       = &EndpointsT::map,
    };

    static constexpr std::array<const FzConverter*, 2> fz_array{{
        &::zhc::lumi::kFzLumiBasic,
        &kFzOnOffBound,
    }};
    // Wall-switch family (QBKG*/ZNQBKG*/WS-EUK*) accepts the same
    // 0xFCC0 attrs as the plug family. Wire them; unsupported models
    // NAK the attribute.
    static constexpr std::array<const TzConverter*, 4> tz_array{{
        &::zhc::generic::kTzOnOff,
        &::zhc::lumi::kTzLumiPowerOutageMemory,
        &::zhc::lumi::kTzLumiLedDisabledNight,
        &::zhc::lumi::kTzLumiButtonLock,
    }};

    static constexpr const FzConverter* const* fz_list = fz_array.data();
    static constexpr std::uint8_t fz_count =
        static_cast<std::uint8_t>(fz_array.size());

    static constexpr const TzConverter* const* tz_list = tz_array.data();
    static constexpr std::uint8_t tz_count =
        static_cast<std::uint8_t>(tz_array.size());
};

// ── LumiAction ────────────────────────────────────────────────────
//
// For Aqara wireless button / remotes using the attr 0x0055 multistate
// action path. Device supplies an `ActionMapT` with:
//   static constexpr LumiActionMap map;
//
// Factory pairs `kFzLumiBasic` (battery TLV) with a
// `fz_lumi_action_multistate` converter bound to `&ActionMapT::map`.
// No outbound path — these are report-only remotes.

template <typename ActionMapT>
struct LumiAction {
    static constexpr FzConverter kFzActionBound{
        .family            = FrameFamily::Zcl,
        .cluster           = "genMultistateInput",
        .type_mask         = type_bit(MessageType::AttributeReport) |
                             type_bit(MessageType::ReadResponse),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &::zhc::lumi::fz_lumi_action_multistate },
        .user_config       = &ActionMapT::map,
    };

    static constexpr std::array<const FzConverter*, 2> fz_array{{
        &::zhc::lumi::kFzLumiBasic,
        &kFzActionBound,
    }};

    static constexpr const FzConverter* const* fz_list = fz_array.data();
    static constexpr std::uint8_t fz_count =
        static_cast<std::uint8_t>(fz_array.size());

    static constexpr const TzConverter* const* tz_list = nullptr;
    static constexpr std::uint8_t tz_count = 0;
};

// ── LumiPower ─────────────────────────────────────────────────────
//
// For Aqara smart plugs / outlets with active-power + energy metering.
// Device supplies a `CalibrationT` with:
//   static constexpr LumiPowerCalibration power;
//   static constexpr LumiMeterCalibration meter;
//
// Bundles: lumi_basic + on_off (relay state) + fz_lumi_power (bound
// to power calibration) + fz_lumi_electricity_meter (bound to meter
// calibration). Outbound: tz_on_off.

template <typename CalibrationT>
struct LumiPower {
    static constexpr FzConverter kFzPowerBound{
        .family            = FrameFamily::Zcl,
        .cluster           = "haElectricalMeasurement",
        .type_mask         = type_bit(MessageType::AttributeReport) |
                             type_bit(MessageType::ReadResponse),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &::zhc::lumi::fz_lumi_power },
        .user_config       = &CalibrationT::power,
    };

    static constexpr FzConverter kFzMeterBound{
        .family            = FrameFamily::Zcl,
        .cluster           = "seMetering",
        .type_mask         = type_bit(MessageType::AttributeReport) |
                             type_bit(MessageType::ReadResponse),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &::zhc::lumi::fz_lumi_electricity_meter },
        .user_config       = &CalibrationT::meter,
    };

    static constexpr std::array<const FzConverter*, 4> fz_array{{
        &::zhc::lumi::kFzLumiBasic,
        &::zhc::generic::kFzOnOff,
        &kFzPowerBound,
        &kFzMeterBound,
    }};
    // Every Aqara metering plug in z2m's modern tz list accepts
    // power_outage_memory + led_disabled_night writes on 0xFCC0.
    // Wiring them unconditionally is harmless on older models — the
    // device simply NAKs an unknown-attr write.
    static constexpr std::array<const TzConverter*, 3> tz_array{{
        &::zhc::generic::kTzOnOff,
        &::zhc::lumi::kTzLumiPowerOutageMemory,
        &::zhc::lumi::kTzLumiLedDisabledNight,
    }};

    static constexpr const FzConverter* const* fz_list = fz_array.data();
    static constexpr std::uint8_t fz_count =
        static_cast<std::uint8_t>(fz_array.size());

    static constexpr const TzConverter* const* tz_list = tz_array.data();
    static constexpr std::uint8_t tz_count =
        static_cast<std::uint8_t>(tz_array.size());
};

}  // namespace zhc::lumi::factory
