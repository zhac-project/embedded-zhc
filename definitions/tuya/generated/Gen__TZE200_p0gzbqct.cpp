// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_p0gzbqct).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_p0gzbqct_light_type_dp4[] = {
    { 0, "led" },
    { 1, "incandescent" },
    { 2, "halogen" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_p0gzbqct[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "min_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "light_type", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_p0gzbqct_light_type_dp4, sizeof(kEnum__TZE200_p0gzbqct_light_type_dp4)/sizeof(kEnum__TZE200_p0gzbqct_light_type_dp4[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_p0gzbqct{ kEntries__TZE200_p0gzbqct, 4 };
constexpr FzConverter kFzDp__TZE200_p0gzbqct{
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
    .user_config       = &kMap__TZE200_p0gzbqct,
};
const FzConverter* const kFz__TZE200_p0gzbqct[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_p0gzbqct,
};
constexpr TzConverter kTzDp__TZE200_p0gzbqct{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_p0gzbqct,
};
const TzConverter* const kTz__TZE200_p0gzbqct[] = { &kTzDp__TZE200_p0gzbqct };
constexpr const char* kM__TZE200_p0gzbqct[] = { "TS0601" };
constexpr const char* kN__TZE200_p0gzbqct[] = { "_TZE200_p0gzbqct" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_p0gzbqct_3[] = {"led", "incandescent", "halogen"};
constexpr const char* kAutoOpts__TZE200_p0gzbqct_4[] = {"none", "relay", "pos"};
constexpr Expose kAutoExposes__TZE200_p0gzbqct[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 0},
    {"min_brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 1, 255, 0},
    {"light_type", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_p0gzbqct_3, 3},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE200_p0gzbqct_4, 3, ExposeCategory::Config},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_p0gzbqct[] = {
    {"Moes","WS-SY-EURD"},
    {"Moes","WS-SY-EURD-WH-MS"},
};
extern const PreparedDefinition kDefGen__TZE200_p0gzbqct{
    .zigbee_models=kM__TZE200_p0gzbqct,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_p0gzbqct,.manufacturer_names_count=1,
    .model="TS0601__TZE200_p0gzbqct",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_p0gzbqct,.exposes_count=sizeof(kAutoExposes__TZE200_p0gzbqct)/sizeof(kAutoExposes__TZE200_p0gzbqct[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_p0gzbqct, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_p0gzbqct)/sizeof(kWhiteLabels_Gen_TZE200_p0gzbqct[0]),
    .from_zigbee=kFz__TZE200_p0gzbqct,
    .from_zigbee_count=sizeof(kFz__TZE200_p0gzbqct)/sizeof(kFz__TZE200_p0gzbqct[0]),
    .to_zigbee=kTz__TZE200_p0gzbqct,
    .to_zigbee_count=sizeof(kTz__TZE200_p0gzbqct)/sizeof(kTz__TZE200_p0gzbqct[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
