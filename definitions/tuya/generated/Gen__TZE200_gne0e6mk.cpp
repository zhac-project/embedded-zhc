// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_gne0e6mk).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gne0e6mk_light_type_dp4[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gne0e6mk_power_on_behavior_dp14[] = {
    { 0, "off" },
    { 1, "on" },
    { 2, "previous" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gne0e6mk_backlight_mode_dp21[] = {
    { 0, "off" },
    { 1, "normal" },
    { 2, "inverted" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_gne0e6mk[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "min_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "light_type", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gne0e6mk_light_type_dp4, sizeof(kEnum__TZE200_gne0e6mk_light_type_dp4)/sizeof(kEnum__TZE200_gne0e6mk_light_type_dp4[0]) },
    { 5, "max_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 6, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gne0e6mk_power_on_behavior_dp14, sizeof(kEnum__TZE200_gne0e6mk_power_on_behavior_dp14)/sizeof(kEnum__TZE200_gne0e6mk_power_on_behavior_dp14[0]) },
    { 21, "backlight_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gne0e6mk_backlight_mode_dp21, sizeof(kEnum__TZE200_gne0e6mk_backlight_mode_dp21)/sizeof(kEnum__TZE200_gne0e6mk_backlight_mode_dp21[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_gne0e6mk{ kEntries__TZE200_gne0e6mk, 8 };
constexpr FzConverter kFzDp__TZE200_gne0e6mk{
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
    .user_config       = &kMap__TZE200_gne0e6mk,
};
const FzConverter* const kFz__TZE200_gne0e6mk[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_gne0e6mk,
};
constexpr TzConverter kTzDp__TZE200_gne0e6mk{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_gne0e6mk,
};
const TzConverter* const kTz__TZE200_gne0e6mk[] = { &kTzDp__TZE200_gne0e6mk };
constexpr const char* kM__TZE200_gne0e6mk[] = { "TS0601" };
constexpr const char* kN__TZE200_gne0e6mk[] = { "_TZE200_gne0e6mk" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_gne0e6mk_5[] = {"off", "previous", "on"};
constexpr const char* kAutoOpts__TZE200_gne0e6mk_6[] = {"led", "incandescent", "halogen"};
constexpr const char* kAutoOpts__TZE200_gne0e6mk_7[] = {"off", "normal", "inverted"};
constexpr Expose kAutoExposes__TZE200_gne0e6mk[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"min_brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"max_brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"countdown", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 43200, 1},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_gne0e6mk_5, 3, ExposeCategory::Config},
    {"light_type", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_gne0e6mk_6, 3},
    {"backlight_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_gne0e6mk_7, 3, ExposeCategory::Config},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_gne0e6mk[] = {
    {"Moes","ZS-SR-EUD-1"},
    {"Moes","MS-105Z"},
    {"Mercator Ikuü","SSWM-DIMZ"},
    {"Zemismart","ZN2S-US1-SD"},
    {"Mercator Ikuü","SSWRM-ZB"},
    {"Lonsonho","EDM-1ZBB-EU"},
    {"ION Industries","ID200W-ZIGB"},
    {"ION Industries","90.500.090"},
    {"ION Industries","90.500.040"},
    {"Lerlink","X706U"},
    {"Moes","ZS-EUD_1gang"},
    {"Larkkey","ZSTY-SM-1DMZG-EU"},
    {"Earda","EDM-1ZAA-EU"},
    {"Earda","EDM-1ZAB-EU"},
    {"Earda","EDM-1ZBA-EU"},
    {"Mercator Ikuü","SSWD01"},
    {"Moes","ZS-USD"},
    {"Moes","EDM-1ZBB-EU"},
};
extern const PreparedDefinition kDefGen__TZE200_gne0e6mk{
    .zigbee_models=kM__TZE200_gne0e6mk,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_gne0e6mk,.manufacturer_names_count=1,
    .model="TS0601__TZE200_gne0e6mk",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_gne0e6mk,.exposes_count=sizeof(kAutoExposes__TZE200_gne0e6mk)/sizeof(kAutoExposes__TZE200_gne0e6mk[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_gne0e6mk, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_gne0e6mk)/sizeof(kWhiteLabels_Gen_TZE200_gne0e6mk[0]),
    .from_zigbee=kFz__TZE200_gne0e6mk,
    .from_zigbee_count=sizeof(kFz__TZE200_gne0e6mk)/sizeof(kFz__TZE200_gne0e6mk[0]),
    .to_zigbee=kTz__TZE200_gne0e6mk,
    .to_zigbee_count=sizeof(kTz__TZE200_gne0e6mk)/sizeof(kTz__TZE200_gne0e6mk[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
