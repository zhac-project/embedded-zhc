// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_0ints6wl).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_0ints6wl_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "middle" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_0ints6wl_water_warning_dp111[] = {
    { 0, "none" },
    { 1, "alarm" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_0ints6wl[] = {
    { 3, "soil_moisture", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_0ints6wl_battery_state_dp14, sizeof(kEnum__TZE284_0ints6wl_battery_state_dp14)/sizeof(kEnum__TZE284_0ints6wl_battery_state_dp14[0]) },
    { 103, "soil_sampling", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "soil_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "humidity_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "illuminance_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 110, "soil_warning", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "water_warning", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_0ints6wl_water_warning_dp111, sizeof(kEnum__TZE284_0ints6wl_water_warning_dp111)/sizeof(kEnum__TZE284_0ints6wl_water_warning_dp111[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_0ints6wl{ kEntries__TZE284_0ints6wl, 12 };
constexpr FzConverter kFzDp__TZE284_0ints6wl{
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
    .user_config       = &kMap__TZE284_0ints6wl,
};
const FzConverter* const kFz__TZE284_0ints6wl[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_0ints6wl,
};
constexpr TzConverter kTzDp__TZE284_0ints6wl{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_0ints6wl,
};
const TzConverter* const kTz__TZE284_0ints6wl[] = { &kTzDp__TZE284_0ints6wl };
constexpr const char* kM__TZE284_0ints6wl[] = { "TS0601" };
constexpr const char* kN__TZE284_0ints6wl[] = { "_TZE284_0ints6wl" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_0ints6wl[] = {
    {"Arteco","ZS-302Z"},
};
extern const PreparedDefinition kDefGen__TZE284_0ints6wl{
    .zigbee_models=kM__TZE284_0ints6wl,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_0ints6wl,.manufacturer_names_count=1,
    .model="TS0601__TZE284_0ints6wl",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE284_0ints6wl, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_0ints6wl)/sizeof(kWhiteLabels_Gen_TZE284_0ints6wl[0]),
    .from_zigbee=kFz__TZE284_0ints6wl,
    .from_zigbee_count=sizeof(kFz__TZE284_0ints6wl)/sizeof(kFz__TZE284_0ints6wl[0]),
    .to_zigbee=kTz__TZE284_0ints6wl,
    .to_zigbee_count=sizeof(kTz__TZE284_0ints6wl)/sizeof(kTz__TZE284_0ints6wl[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
