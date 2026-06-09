// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Neo NAS-PD07 — PIR motion + temperature & humidity sensor.
// Graduated from generated/ during the neo z2m<->embedded-zhc parity pass.
// z2m serves _TZE200_7hfcudw5 via the legacy Tuya-DP converter
// `legacy.fz.neo_nas_pd07` (manuSpecificTuya 0xEF00), NOT IAS Zone. The
// generated def wired kFzIasZone + a {alarm,tamper,battery_low} IAS expose
// set, so occupancy/temperature/humidity/tamper never decoded. Replaced with
// the Tuya-DP map mirroring z2m's neo_nas_pd07 DP switch. occupancy/tamper
// follow the established Tuya PIR Bool convention (wire bool true => detected),
// matching z2m's `value > 0`.
// z2m-source: neo.ts #NAS-PD07 + lib/legacy.ts fz.neo_nas_pd07.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::neo {
namespace {

// z2m power_type DP102: {0:battery_full,1:battery_high,2:battery_medium,
//                        3:battery_low,4:usb}.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_PD07_power_type[] = {
    { 0, "battery_full" },
    { 1, "battery_high" },
    { 2, "battery_medium" },
    { 3, "battery_low" },
    { 4, "usb" },
};

// z2m temperature_scale DP106: `value ? "°C" : "°F"`.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_PD07_temp_scale[] = {
    { 0, "°F" },
    { 1, "°C" },
};

// z2m alarm DP113 (neoTempHumidityAlarm):
//   {0:over_temperature,1:over_humidity,2:below_min_temperature,
//    3:below_min_humdity,4:off}.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_PD07_alarm[] = {
    { 0, "over_temperature" },
    { 1, "over_humidity" },
    { 2, "below_min_temperature" },
    { 3, "below_min_humdity" },
    { 4, "off" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_PD07[] = {
    { 101, "occupancy",       ::zhc::TuyaDpType::Bool,    1, nullptr, 0 },             // neoOccupancy (value>0)
    { 102, "power_type",      ::zhc::TuyaDpType::Enum,    1, kEnum_PD07_power_type, 5 },
    { 103, "tamper",          ::zhc::TuyaDpType::Bool,    1, nullptr, 0 },             // neoTamper (value>0)
    { 104, "temperature",     ::zhc::TuyaDpType::Numeric, 10, nullptr, 0 },            // z2m: value/10
    { 105, "humidity",        ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 106, "temperature_scale", ::zhc::TuyaDpType::Enum,  1, kEnum_PD07_temp_scale, 2 },
    { 107, "temperature_min", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 108, "temperature_max", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 109, "humidity_min",    ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 110, "humidity_max",    ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 113, "alarm",           ::zhc::TuyaDpType::Enum,    1, kEnum_PD07_alarm, 5 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_PD07{
    kEntries_PD07, sizeof(kEntries_PD07) / sizeof(kEntries_PD07[0]) };

constexpr FzConverter kFzDp_PD07{
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
    .user_config       = &kMap_PD07,
};
const FzConverter* const kFz_NAS_PD07[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_PD07,
};

constexpr TzConverter kTzDp_PD07{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_PD07,
};
const TzConverter* const kTz_NAS_PD07[] = { &kTzDp_PD07 };

constexpr const char* kModels_NAS_PD07[] = { "TS0601" };
constexpr const char* kManus_NAS_PD07[] = { "_TZE200_7hfcudw5" };

constexpr Expose kExposes_NAS_PD07[] = {
    {"occupancy",       ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"temperature",     ExposeType::Numeric, Access::State,    "°C", nullptr, nullptr, 0},
    {"humidity",        ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"tamper",          ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"power_type",      ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"temperature_min", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0},
    {"temperature_max", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0},
    {"humidity_min",    ExposeType::Numeric, Access::StateSet, "%",  nullptr, nullptr, 0},
    {"humidity_max",    ExposeType::Numeric, Access::StateSet, "%",  nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_NAS_PD07[] = {
    {1, 0xEF00},
};
}  // namespace

extern const PreparedDefinition kDef_NAS_PD07{
    .zigbee_models=kModels_NAS_PD07, .zigbee_models_count=sizeof(kModels_NAS_PD07)/sizeof(kModels_NAS_PD07[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_NAS_PD07, .manufacturer_names_count=sizeof(kManus_NAS_PD07)/sizeof(kManus_NAS_PD07[0]),
    .model="NAS-PD07", .vendor="Neo",
    .meta=nullptr, .exposes=kExposes_NAS_PD07, .exposes_count=sizeof(kExposes_NAS_PD07)/sizeof(kExposes_NAS_PD07[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NAS_PD07, .from_zigbee_count=sizeof(kFz_NAS_PD07)/sizeof(kFz_NAS_PD07[0]),
    .to_zigbee=kTz_NAS_PD07, .to_zigbee_count=sizeof(kTz_NAS_PD07)/sizeof(kTz_NAS_PD07[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBindings_NAS_PD07, .bindings_count=sizeof(kBindings_NAS_PD07)/sizeof(kBindings_NAS_PD07[0]),
};

}  // namespace zhc::devices::neo
