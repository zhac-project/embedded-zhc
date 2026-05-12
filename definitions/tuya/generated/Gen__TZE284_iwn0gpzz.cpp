// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_iwn0gpzz).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_iwn0gpzz[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 23, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 32, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 50, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 103, "current", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 104, "power", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_iwn0gpzz{ kEntries__TZE284_iwn0gpzz, 7 };
constexpr FzConverter kFzDp__TZE284_iwn0gpzz{
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
    .user_config       = &kMap__TZE284_iwn0gpzz,
};
const FzConverter* const kFz__TZE284_iwn0gpzz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_iwn0gpzz,
};
constexpr TzConverter kTzDp__TZE284_iwn0gpzz{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_iwn0gpzz,
};
const TzConverter* const kTz__TZE284_iwn0gpzz[] = { &kTzDp__TZE284_iwn0gpzz };
constexpr const char* kM__TZE284_iwn0gpzz[] = { "TS0601" };
constexpr const char* kN__TZE284_iwn0gpzz[] = { "_TZE284_iwn0gpzz" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_iwn0gpzz{
    .zigbee_models=kM__TZE284_iwn0gpzz,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_iwn0gpzz,.manufacturer_names_count=1,
    .model="TS0601__TZE284_iwn0gpzz",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_iwn0gpzz,
    .from_zigbee_count=sizeof(kFz__TZE284_iwn0gpzz)/sizeof(kFz__TZE284_iwn0gpzz[0]),
    .to_zigbee=kTz__TZE284_iwn0gpzz,
    .to_zigbee_count=sizeof(kTz__TZE284_iwn0gpzz)/sizeof(kTz__TZE284_iwn0gpzz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
