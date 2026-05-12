// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Tuya factories — thin wrappers that bundle mcuSyncTime + a
// `fz_tuya_datapoints` converter pre-bound to a per-device
// `TuyaDatapointMap`.
//
// Each factory takes a type template parameter with a
// `static constexpr TuyaDatapointMap dp_map;` member. Device usage:
//
//   struct simple_plug_cfg {
//       static constexpr tuya::TuyaDpMapEntry entries[] = {
//           tuya::dp::binary(1, "state"),
//       };
//       static constexpr tuya::TuyaDatapointMap dp_map{entries, 1};
//   };
//   using FX = factory::TuyaOnOff<simple_plug_cfg>;
//
// z2m-source: tuya.ts `tuya.modernExtend.tuyaOnOff` /
//             `tuya.modernExtend.tuyaLight`.

#include <array>
#include <cstddef>
#include <cstdint>

#include "definitions/tuya/_shared.hpp"

namespace zhc::tuya::factory {

namespace detail {

template <typename CfgT>
struct BoundFz {
    static constexpr FzConverter converter{
        .family            = FrameFamily::TuyaDp,
        .cluster           = "manuSpecificTuya",
        .type_mask         = type_bit(MessageType::Command),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
        .user_config       = &CfgT::dp_map,
    };
};

}  // namespace detail

// `tuyaOnOff<Cfg>` — simple-switch bundle. mcuSyncTime + DP map.
template <typename CfgT>
struct TuyaOnOff {
    static constexpr std::array<const FzConverter*, 2> fz_array{{
        &kFzTuyaMcuSyncTime,
        &detail::BoundFz<CfgT>::converter,
    }};
    static constexpr const FzConverter* const* fz_list = fz_array.data();
    static constexpr std::uint8_t fz_count =
        static_cast<std::uint8_t>(fz_array.size());
    static constexpr const TzConverter* const* tz_list = nullptr;
    static constexpr std::uint8_t tz_count = 0;
};

// `tuyaLight<Cfg>` — same bundle shape. Distinct template so the
// adapter can distinguish light vs switch by type identity when
// picking canonical `configureReporting` paths.
template <typename CfgT>
struct TuyaLight {
    static constexpr std::array<const FzConverter*, 2> fz_array{{
        &kFzTuyaMcuSyncTime,
        &detail::BoundFz<CfgT>::converter,
    }};
    static constexpr const FzConverter* const* fz_list = fz_array.data();
    static constexpr std::uint8_t fz_count =
        static_cast<std::uint8_t>(fz_array.size());
    static constexpr const TzConverter* const* tz_list = nullptr;
    static constexpr std::uint8_t tz_count = 0;
};

}  // namespace zhc::tuya::factory
