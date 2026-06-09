// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Neo NAS-AB02B0 — temperature & humidity sensor with alarm/siren.
// Graduated from generated/ during the neo z2m<->embedded-zhc parity pass.
// z2m serves _TZE200_d0yu2xgi via the legacy Tuya-DP converter
// `legacy.fz.neo_t_h_alarm` (manuSpecificTuya 0xEF00), NOT IAS Zone. The
// generated def wired kFzIasZone + a {alarm,tamper,battery_low} IAS expose
// set, so temperature/humidity/alarm/duration/volume never decoded. Replaced
// with the proper Tuya-DP map mirroring z2m's neo_t_h_alarm DP switch.
// z2m-source: neo.ts #NAS-AB02B0 + lib/legacy.ts fz.neo_t_h_alarm
//             (dataPoints.neo* DP numbers).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::neo {
namespace {

// z2m fz.neo_t_h_alarm: volume DP116 maps {2:low, 1:medium, 0:high}.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_AB02B0_volume[] = {
    { 2, "low" },
    { 1, "medium" },
    { 0, "high" },
};

// z2m power_type DP101: {0:battery_full,1:battery_high,2:battery_medium,
//                        3:battery_low,4:usb}.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_AB02B0_power_type[] = {
    { 0, "battery_full" },
    { 1, "battery_high" },
    { 2, "battery_medium" },
    { 3, "battery_low" },
    { 4, "usb" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_AB02B0[] = {
    { 101, "power_type",        ::zhc::TuyaDpType::Enum,    1, kEnum_AB02B0_power_type, 5 },
    { 102, "melody",            ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 103, "duration",          ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 104, "alarm",             ::zhc::TuyaDpType::Bool,    1, nullptr, 0 },
    { 105, "temperature",       ::zhc::TuyaDpType::Numeric, 10, nullptr, 0 },  // z2m: value/10
    { 106, "humidity",          ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 107, "temperature_min",   ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 108, "temperature_max",   ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 109, "humidity_min",      ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 110, "humidity_max",      ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },
    { 113, "temperature_alarm", ::zhc::TuyaDpType::Bool,    1, nullptr, 0 },
    { 114, "humidity_alarm",    ::zhc::TuyaDpType::Bool,    1, nullptr, 0 },
    { 116, "volume",            ::zhc::TuyaDpType::Enum,    1, kEnum_AB02B0_volume, 3 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_AB02B0{
    kEntries_AB02B0, sizeof(kEntries_AB02B0) / sizeof(kEntries_AB02B0[0]) };

constexpr FzConverter kFzDp_AB02B0{
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
    .user_config       = &kMap_AB02B0,
};
const FzConverter* const kFz_NAS_AB02B0[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_AB02B0,
};

constexpr TzConverter kTzDp_AB02B0{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_AB02B0,
};
const TzConverter* const kTz_NAS_AB02B0[] = { &kTzDp_AB02B0 };

constexpr const char* kModels_NAS_AB02B0[] = { "0yu2xgi", "TS0601" };
constexpr const char* kManus_NAS_AB02B0[] = { "_TZE200_d0yu2xgi" };

constexpr Expose kExposes_NAS_AB02B0[] = {
    {"temperature",       ExposeType::Numeric, Access::State,    "°C", nullptr, nullptr, 0},
    {"humidity",          ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"alarm",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature_alarm", ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity_alarm",    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"duration",          ExposeType::Numeric, Access::StateSet, "s",  nullptr, nullptr, 0},
    {"temperature_min",   ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0},
    {"temperature_max",   ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0},
    {"humidity_min",      ExposeType::Numeric, Access::StateSet, "%",  nullptr, nullptr, 0},
    {"humidity_max",      ExposeType::Numeric, Access::StateSet, "%",  nullptr, nullptr, 0},
    {"melody",            ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"volume",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_type",        ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_NAS_AB02B0[] = {
    {1, 0xEF00},
};
}  // namespace

extern const PreparedDefinition kDef_NAS_AB02B0{
    .zigbee_models=kModels_NAS_AB02B0, .zigbee_models_count=sizeof(kModels_NAS_AB02B0)/sizeof(kModels_NAS_AB02B0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_NAS_AB02B0, .manufacturer_names_count=sizeof(kManus_NAS_AB02B0)/sizeof(kManus_NAS_AB02B0[0]),
    .model="NAS-AB02B0", .vendor="Neo",
    .meta=nullptr, .exposes=kExposes_NAS_AB02B0, .exposes_count=sizeof(kExposes_NAS_AB02B0)/sizeof(kExposes_NAS_AB02B0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NAS_AB02B0, .from_zigbee_count=sizeof(kFz_NAS_AB02B0)/sizeof(kFz_NAS_AB02B0[0]),
    .to_zigbee=kTz_NAS_AB02B0, .to_zigbee_count=sizeof(kTz_NAS_AB02B0)/sizeof(kTz_NAS_AB02B0[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBindings_NAS_AB02B0, .bindings_count=sizeof(kBindings_NAS_AB02B0)/sizeof(kBindings_NAS_AB02B0[0]),
};

}  // namespace zhc::devices::neo
