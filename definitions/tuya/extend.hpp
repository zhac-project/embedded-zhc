// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Tuya modern-extend helpers. Thin host-side stubs; identity of each
// ConfigureFn lets the platform adapter know which canned Tuya setup
// step to apply (register cluster 0xEF00, send magic packet, skip
// configureReporting, etc.).
//
// z2m-source: tuya.ts `tuya.modernExtend.*`.

#include <cstdint>

#include "zhc/runtime/definition.hpp"

namespace zhc::tuya::extend {

namespace detail {
inline void register_cluster_fn(std::uint16_t, RuntimeContext&) {}
inline void tuya_base_configure_fn(std::uint16_t, RuntimeContext&) {}
inline void magic_packet_fn(std::uint16_t, RuntimeContext&) {}
inline void electricity_meter_poll_fn(std::uint16_t, RuntimeContext&) {}
}  // namespace detail

// `electricity_meter_poll()` — sentinel ConfigureFn. Adapter reads
// the function identity to schedule periodic `haElectricalMeasurement`
// reads via `RuntimeContext::timers`.
constexpr ConfigureFn electricity_meter_poll() {
    return &detail::electricity_meter_poll_fn;
}

// `tuya_base_configure()` → ConfigureFn stub. Bundles:
//   • cluster registration (0xEF00, sometimes 0xE001),
//   • skip default configureReporting,
//   • optional magic packet on join.
// Platform adapter reads the function identity to decide what to do.
constexpr ConfigureFn tuya_base_configure() {
    return &detail::tuya_base_configure_fn;
}

// `register_cluster()` → standalone cluster-register hook.
constexpr ConfigureFn register_cluster() {
    return &detail::register_cluster_fn;
}

// `magic_packet()` → send a wake-up sequence on join. Per-device
// payloads live in `user_config` on the owning PreparedDefinition's
// `meta`; adapter sniffs by function identity.
constexpr ConfigureFn magic_packet() {
    return &detail::magic_packet_fn;
}

// Magic-packet payloads referenced by PreparedDefinition::meta when
// the device uses `magic_packet()` as its configure hook.

struct TuyaMagicPacket {
    const std::uint8_t* bytes;
    std::uint16_t       len;
    std::uint16_t       cluster_id;     // 0 means "no cluster preamble"
};

inline constexpr std::uint8_t kMagicReadAttrsBytes[] = {
    0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x05, 0x00, 0x07,
};
inline constexpr TuyaMagicPacket kTuyaMagicPacket_readAttrs{
    kMagicReadAttrsBytes,
    static_cast<std::uint16_t>(sizeof(kMagicReadAttrsBytes)),
    /*cluster_id=*/0x0000,
};

inline constexpr std::uint8_t kMagicZoneModeBytes[] = {
    0x01, 0x00,
};
inline constexpr TuyaMagicPacket kTuyaMagicPacket_zoneMode{
    kMagicZoneModeBytes,
    static_cast<std::uint16_t>(sizeof(kMagicZoneModeBytes)),
    /*cluster_id=*/0xE001,
};

}  // namespace zhc::tuya::extend
