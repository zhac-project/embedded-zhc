// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HOBEIAN ZG-204ZK 24GHz presence sensor (z2m v26.77.0 parity).
// z2m-source: tuya.ts #ZG-204ZK
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

// DP map mirrors z2m v26.77.0 meta.tuyaDatapoints for ZG-204ZK (basis:
// emit/out/ZG_204ZK.dpmap.cpp — byte-for-byte). Divisors verified against the
// valueConverters: presence=trueFalse1 (non-inverted bool), fading_time=raw,
// detection_distance=divideBy100, static/motion sensitivity=raw, indicator &
// anti_interference=onOff (non-inverted bool), battery=raw.
constexpr ::zhc::tuya::TuyaDpMapEntry kDpMap_ZG_204ZK[] = {
    ::zhc::tuya::dp::binary(1, "presence"),
    ::zhc::tuya::dp::numeric(102, "fading_time", 1),
    ::zhc::tuya::dp::numeric(4, "detection_distance", 100),
    ::zhc::tuya::dp::numeric(2, "static_detection_sensitivity", 1),
    ::zhc::tuya::dp::binary(107, "indicator"),
    ::zhc::tuya::dp::numeric(123, "motion_detection_sensitivity", 1),
    ::zhc::tuya::dp::numeric(121, "battery", 1),
    ::zhc::tuya::dp::binary(122, "anti_interference"),
};
constexpr ::zhc::tuya::TuyaDatapointMap kDpMapWrap_ZG_204ZK{
    kDpMap_ZG_204ZK, sizeof(kDpMap_ZG_204ZK) / sizeof(kDpMap_ZG_204ZK[0]) };

constexpr FzConverter kFzDp_ZG_204ZK{
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
    .user_config       = &kDpMapWrap_ZG_204ZK,
};
const FzConverter* const kFz_ZG_204ZK[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ZG_204ZK,
};
constexpr TzConverter kTzDp_ZG_204ZK{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kDpMapWrap_ZG_204ZK,
};
const TzConverter* const kTz_ZG_204ZK[] = { &kTzDp_ZG_204ZK };

constexpr Expose kExp_ZG_204ZK[] = {
    { "presence",                     ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Presence state",               nullptr, 0 },
    { "battery",                      ExposeType::Numeric, ::zhc::Access::State,    "%",     "Battery percentage",           nullptr, 0 },
    { "fading_time",                  ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Presence keep time",           nullptr, 0 },
    { "detection_distance",           ExposeType::Numeric, ::zhc::Access::StateSet, "m",     "Detection distance",           nullptr, 0 },
    { "static_detection_sensitivity", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Static detection sensitivity", nullptr, 0 },
    { "motion_detection_sensitivity", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Motion detection sensitivity", nullptr, 0 },
    { "indicator",                    ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "LED indicator mode",           nullptr, 0 },
    { "anti_interference",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Anti interference function",   nullptr, 0 },
};

constexpr const char* kM_ZG_204ZK[] = { "ZG-204ZK", "AY-204ZX" };
constexpr const char* kN_ZG_204ZK[] = { "_TZE200_ka8l86iu", "_TZE200_zbfmvj13" };
}  // namespace

extern const PreparedDefinition kDef_ZG_204ZK{
    .zigbee_models=kM_ZG_204ZK,.zigbee_models_count=sizeof(kM_ZG_204ZK)/sizeof(kM_ZG_204ZK[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_ZG_204ZK,.manufacturer_names_count=sizeof(kN_ZG_204ZK)/sizeof(kN_ZG_204ZK[0]),
    .model="ZG-204ZK",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp_ZG_204ZK,.exposes_count=sizeof(kExp_ZG_204ZK)/sizeof(kExp_ZG_204ZK[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz_ZG_204ZK,.from_zigbee_count=sizeof(kFz_ZG_204ZK)/sizeof(kFz_ZG_204ZK[0]),
    .to_zigbee=kTz_ZG_204ZK,.to_zigbee_count=sizeof(kTz_ZG_204ZK)/sizeof(kTz_ZG_204ZK[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
