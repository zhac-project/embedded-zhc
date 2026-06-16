// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601 / _TZE204_upagmta9 — temperature & humidity sensor (ZTH-series).
//
// Graduated from generated/Gen__TZE204_upagmta9.cpp to add EXPOSES WITH UNITS. The
// generated def shipped `.exposes=nullptr`, so device.get returned an empty exposes
// array → neither the local web-ui (which renders units from the device's exposes) nor
// the cloud (which resolves capabilities from those exposes) could show the °/% units —
// only WSD500A, which had explicit exposes, did. The DP decode (temperature /10,
// humidity, battery_state, temperature_unit) is byte-for-byte the generated def's; this
// only declares the exposes so the units render everywhere. Pure temp/humidity sensor →
// the cloud classifies it `climate_sensor`.
//
// z2m-source: tuya.ts fingerprint TS0601 / _TZE204_upagmta9.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_upagmta9_battery_state_dp3[] = {
    { 0, "low" }, { 1, "medium" }, { 2, "high" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_upagmta9_temperature_unit_dp9[] = {
    { 0, "celsius" }, { 1, "fahrenheit" },
};
constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_upagmta9[] = {
    { 1, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 2, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_upagmta9_battery_state_dp3, sizeof(kEnum__TZE204_upagmta9_battery_state_dp3)/sizeof(kEnum__TZE204_upagmta9_battery_state_dp3[0]) },
    { 9, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_upagmta9_temperature_unit_dp9, sizeof(kEnum__TZE204_upagmta9_temperature_unit_dp9)/sizeof(kEnum__TZE204_upagmta9_temperature_unit_dp9[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_upagmta9{ kEntries__TZE204_upagmta9, 4 };
constexpr FzConverter kFzDp__TZE204_upagmta9{
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
    .user_config       = &kMap__TZE204_upagmta9,
};
const FzConverter* const kFz__TZE204_upagmta9[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_upagmta9,
};
constexpr TzConverter kTzDp__TZE204_upagmta9{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_upagmta9,
};
const TzConverter* const kTz__TZE204_upagmta9[] = { &kTzDp__TZE204_upagmta9 };

constexpr const char* kBatteryStateVals__TZE204_upagmta9[] = { "low", "medium", "high" };
constexpr Expose kExp__TZE204_upagmta9[] = {
    { "temperature",   ExposeType::Numeric, ::zhc::Access::State, "C", nullptr, nullptr, 0 },
    { "humidity",      ExposeType::Numeric, ::zhc::Access::State, "%", nullptr, nullptr, 0 },
    { "battery_state", ExposeType::Enum,    ::zhc::Access::State, nullptr, nullptr,
                       kBatteryStateVals__TZE204_upagmta9,
                       sizeof(kBatteryStateVals__TZE204_upagmta9)/sizeof(kBatteryStateVals__TZE204_upagmta9[0]) },
};

constexpr const char* kM__TZE204_upagmta9[] = { "TS0601" };
constexpr const char* kN__TZE204_upagmta9[] = { "_TZE204_upagmta9" };
constexpr WhiteLabel kWhiteLabels__TZE204_upagmta9[] = {
    {"Tuya","ZTH01"}, {"Tuya","SZTH02"}, {"Tuya","ZTH02"},
    {"Tuya","ZTH05"}, {"Tuya","ZTH08-E"}, {"Tuya","ZTH08"},
};
}  // namespace

extern const PreparedDefinition kDef_TS0601_TZE204_upagmta9{
    .zigbee_models=kM__TZE204_upagmta9,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_upagmta9,.manufacturer_names_count=1,
    .model="TS0601__TZE204_upagmta9",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp__TZE204_upagmta9,.exposes_count=sizeof(kExp__TZE204_upagmta9)/sizeof(kExp__TZE204_upagmta9[0]),
    .white_labels=kWhiteLabels__TZE204_upagmta9, .white_labels_count=sizeof(kWhiteLabels__TZE204_upagmta9)/sizeof(kWhiteLabels__TZE204_upagmta9[0]),
    .from_zigbee=kFz__TZE204_upagmta9,
    .from_zigbee_count=sizeof(kFz__TZE204_upagmta9)/sizeof(kFz__TZE204_upagmta9[0]),
    .to_zigbee=kTz__TZE204_upagmta9,
    .to_zigbee_count=sizeof(kTz__TZE204_upagmta9)/sizeof(kTz__TZE204_upagmta9[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
