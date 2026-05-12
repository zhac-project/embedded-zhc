// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// ── Lumi utility extend helpers ────────────────────────────────────
//
// Thin wrappers matching z2m's `lumiPreventReset`, `lumiFlipIndicator
// Light`, `lumiLedDisabledNight`, `addManuSpecificLumiCluster`,
// `lumiZigbeeOTA`, `lumiElectricityMeter`, `lumiModernExtend`.
//
// On the host these are no-ops; identity matters. Each
// `ConfigureFn`-returning helper yields a distinct function pointer so
// the platform adapter can sniff which canned behaviour to apply.
// Metadata-tag helpers return pointers to constexpr marker objects
// that the adapter compares against.
//
// z2m-source: zigbee-herdsman-converters/src/lib/lumi.ts.

#include <cstdint>

#include "zhc/runtime/definition.hpp"

namespace zhc::lumi::extend {

namespace detail {
inline void prevent_reset_fn(std::uint16_t, RuntimeContext&) {}
inline void flip_indicator_light_fn(std::uint16_t, RuntimeContext&) {}
inline void led_disabled_night_fn(std::uint16_t, RuntimeContext&) {}
inline void add_manu_specific_lumi_cluster_fn(std::uint16_t,
                                                RuntimeContext&) {}
inline void zigbee_ota_fn(std::uint16_t, RuntimeContext&) {}
inline void electricity_meter_fn(std::uint16_t, RuntimeContext&) {}
}  // namespace detail

// `configure`-style stubs. Unique function pointers let the platform
// adapter match on identity (if fn == lumi_prevent_reset → write
// genBasic 0xFFF0, etc.).

constexpr ConfigureFn prevent_reset()            { return &detail::prevent_reset_fn; }
constexpr ConfigureFn flip_indicator_light()     { return &detail::flip_indicator_light_fn; }
constexpr ConfigureFn led_disabled_night()       { return &detail::led_disabled_night_fn; }
constexpr ConfigureFn add_manu_specific_lumi_cluster() {
    return &detail::add_manu_specific_lumi_cluster_fn;
}
constexpr ConfigureFn zigbee_ota()               { return &detail::zigbee_ota_fn; }
constexpr ConfigureFn electricity_meter_config() { return &detail::electricity_meter_fn; }

// Composite "modernExtend" — the 5-tuple z2m bundles for most Aqara
// devices. Returns an array of ConfigureFns that the device's own
// configure function can invoke in sequence. Keeping this as an array
// (not a single composite ConfigureFn) lets the adapter reason about
// each step independently.

struct LumiModernExtendSteps {
    ConfigureFn steps[5];
    std::uint8_t count;
};

constexpr LumiModernExtendSteps lumi_modern_extend_default() {
    return {
        { prevent_reset(),
          flip_indicator_light(),
          led_disabled_night(),
          add_manu_specific_lumi_cluster(),
          zigbee_ota() },
        5,
    };
}

}  // namespace zhc::lumi::extend
