// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS000F / _TZ3218_7fiyo3kv).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZ3218_7fiyo3kv[] = {
    { 108, "temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "humidity_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "temperature_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 112, "humidity_sensitivity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "manual_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZ3218_7fiyo3kv{ kEntries__TZ3218_7fiyo3kv, 5 };
constexpr FzConverter kFzDp__TZ3218_7fiyo3kv{
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
    .user_config       = &kMap__TZ3218_7fiyo3kv,
};
const FzConverter* const kFz__TZ3218_7fiyo3kv[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZ3218_7fiyo3kv,
};
constexpr TzConverter kTzDp__TZ3218_7fiyo3kv{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZ3218_7fiyo3kv,
};
const TzConverter* const kTz__TZ3218_7fiyo3kv[] = { &kTzDp__TZ3218_7fiyo3kv };
constexpr const char* kM__TZ3218_7fiyo3kv[] = { "TS000F" };
constexpr const char* kN__TZ3218_7fiyo3kv[] = { "_TZ3218_7fiyo3kv" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZ3218_7fiyo3kv_1[] = {"on", "off", "restore"};
constexpr Expose kAutoExposes__TZ3218_7fiyo3kv[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_outage_memory", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZ3218_7fiyo3kv_1, 3, ExposeCategory::Config},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -10, 10, 0},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, -10, 10, 0},
    {"temperature_sensitivity", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 1, 0},
    {"humidity_sensitivity", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 1, 10, 0},
    {"manual_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZ3218_7fiyo3kv{
    .zigbee_models=kM__TZ3218_7fiyo3kv,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZ3218_7fiyo3kv,.manufacturer_names_count=1,
    .model="TS000F__TZ3218_7fiyo3kv",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZ3218_7fiyo3kv,.exposes_count=sizeof(kAutoExposes__TZ3218_7fiyo3kv)/sizeof(kAutoExposes__TZ3218_7fiyo3kv[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZ3218_7fiyo3kv,
    .from_zigbee_count=sizeof(kFz__TZ3218_7fiyo3kv)/sizeof(kFz__TZ3218_7fiyo3kv[0]),
    .to_zigbee=kTz__TZ3218_7fiyo3kv,
    .to_zigbee_count=sizeof(kTz__TZ3218_7fiyo3kv)/sizeof(kTz__TZ3218_7fiyo3kv[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
