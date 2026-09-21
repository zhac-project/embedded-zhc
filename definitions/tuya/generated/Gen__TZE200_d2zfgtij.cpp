// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_d2zfgtij).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_d2zfgtij[] = {
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 23, "produced_energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 32, "ac_frequency", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 50, "power_factor", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "device_locating", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_d2zfgtij{ kEntries__TZE200_d2zfgtij, 5 };
constexpr FzConverter kFzDp__TZE200_d2zfgtij{
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
    .user_config       = &kMap__TZE200_d2zfgtij,
};
const FzConverter* const kFz__TZE200_d2zfgtij[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_d2zfgtij,
};
constexpr TzConverter kTzDp__TZE200_d2zfgtij{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_d2zfgtij,
};
const TzConverter* const kTz__TZE200_d2zfgtij[] = { &kTzDp__TZE200_d2zfgtij };
constexpr const char* kM__TZE200_d2zfgtij[] = { "TS0601" };
constexpr const char* kN__TZE200_d2zfgtij[] = { "_TZE200_d2zfgtij" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE200_d2zfgtij[] = {
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_factor", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"ac_frequency", ExposeType::Numeric, Access::State, "Hz", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"device_locating", ExposeType::Binary, Access::Set, nullptr, nullptr, nullptr, 0},
    {"update_frequency", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 5, 3600, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_d2zfgtij{
    .zigbee_models=kM__TZE200_d2zfgtij,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_d2zfgtij,.manufacturer_names_count=1,
    .model="TS0601__TZE200_d2zfgtij",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_d2zfgtij,.exposes_count=sizeof(kAutoExposes__TZE200_d2zfgtij)/sizeof(kAutoExposes__TZE200_d2zfgtij[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_d2zfgtij,
    .from_zigbee_count=sizeof(kFz__TZE200_d2zfgtij)/sizeof(kFz__TZE200_d2zfgtij[0]),
    .to_zigbee=kTz__TZE200_d2zfgtij,
    .to_zigbee_count=sizeof(kTz__TZE200_d2zfgtij)/sizeof(kTz__TZE200_d2zfgtij[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
