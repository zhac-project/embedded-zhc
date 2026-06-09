// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Neo NAS-AB02B2 — alarm/siren (no on-board temp/humidity sensor).
// Graduated from generated/ during the neo z2m<->embedded-zhc parity pass.
// z2m serves _TZE200_t1blo2bj / _TZE204_t1blo2bj / _TZE204_q76rtoa9 via the
// legacy Tuya-DP converter `legacy.fz.neo_alarm` (manuSpecificTuya 0xEF00),
// NOT IAS Zone. The generated def wired kFzIasZone + a {alarm,tamper,
// battery_low} IAS expose set, so alarm/duration/melody/volume/battery never
// decoded. Replaced with the Tuya-DP map mirroring z2m's neo_alarm DP switch
// (note: these DP ids differ from NAS-AB02B0's neo_t_h_alarm).
// z2m-source: neo.ts #NAS-AB02B2 + lib/legacy.ts fz.neo_alarm
//             (dataPoints.neoAO* DP numbers).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::neo {
namespace {

// z2m fz.neo_alarm: volume DP5 maps {0:low, 1:medium, 2:high}.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_AB02B2_volume[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_AB02B2[] = {
    { 5,  "volume",        ::zhc::TuyaDpType::Enum,    1, kEnum_AB02B2_volume, 3 },  // neoAOVolume
    { 7,  "duration",      ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },              // neoAODuration
    { 13, "alarm",         ::zhc::TuyaDpType::Bool,    1, nullptr, 0 },              // neoAOAlarm
    { 15, "battpercentage", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },             // neoAOBattPerc
    { 21, "melody",        ::zhc::TuyaDpType::Numeric, 1, nullptr, 0 },              // neoAOMelody
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_AB02B2{
    kEntries_AB02B2, sizeof(kEntries_AB02B2) / sizeof(kEntries_AB02B2[0]) };

constexpr FzConverter kFzDp_AB02B2{
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
    .user_config       = &kMap_AB02B2,
};
const FzConverter* const kFz_NAS_AB02B2[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_AB02B2,
};

constexpr TzConverter kTzDp_AB02B2{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_AB02B2,
};
const TzConverter* const kTz_NAS_AB02B2[] = { &kTzDp_AB02B2 };

constexpr const char* kModels_NAS_AB02B2[] = { "1blo2bj", "lrfgpny", "q76rtoa9", "TS0601" };
constexpr const char* kManus_NAS_AB02B2[] = { "_TZE200_t1blo2bj", "_TZE204_t1blo2bj", "_TZE204_q76rtoa9" };

constexpr Expose kExposes_NAS_AB02B2[] = {
    {"alarm",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"duration",       ExposeType::Numeric, Access::StateSet, "s",  nullptr, nullptr, 0},
    {"volume",         ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"melody",         ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battpercentage", ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_NAS_AB02B2[] = {
    {1, 0xEF00},
};
}  // namespace

extern const PreparedDefinition kDef_NAS_AB02B2{
    .zigbee_models=kModels_NAS_AB02B2, .zigbee_models_count=sizeof(kModels_NAS_AB02B2)/sizeof(kModels_NAS_AB02B2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_NAS_AB02B2, .manufacturer_names_count=sizeof(kManus_NAS_AB02B2)/sizeof(kManus_NAS_AB02B2[0]),
    .model="NAS-AB02B2", .vendor="Neo",
    .meta=nullptr, .exposes=kExposes_NAS_AB02B2, .exposes_count=sizeof(kExposes_NAS_AB02B2)/sizeof(kExposes_NAS_AB02B2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NAS_AB02B2, .from_zigbee_count=sizeof(kFz_NAS_AB02B2)/sizeof(kFz_NAS_AB02B2[0]),
    .to_zigbee=kTz_NAS_AB02B2, .to_zigbee_count=sizeof(kTz_NAS_AB02B2)/sizeof(kTz_NAS_AB02B2[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBindings_NAS_AB02B2, .bindings_count=sizeof(kBindings_NAS_AB02B2)/sizeof(kBindings_NAS_AB02B2[0]),
};

}  // namespace zhc::devices::neo
