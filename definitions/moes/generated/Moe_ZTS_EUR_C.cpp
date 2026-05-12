// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Moes ZTS-EUR-C — auto-generated.
// Zigbee + RF curtain switch
// z2m-source: moes.ts #ZTS-EUR-C.
// Phase 2 (2026-04-27): wired Tuya DP map from legacy.fz.moes_cover
// (legacy.ts:3091-3132). DPs: 1 state, 2 position, 3 calibration,
// 7 backlight, 8 motor_reversal.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_EURC_state[] = {
    { 0, "OPEN" }, { 1, "STOP" }, { 2, "CLOSE" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_EURC_calibration[] = {
    { 0, "ON" }, { 1, "OFF" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_EURC_motor_reversal[] = {
    { 0, "OFF" }, { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_EURC[] = {
    { 1, "state",          ::zhc::TuyaDpType::Enum, 1,
       kEnum_EURC_state, sizeof(kEnum_EURC_state)/sizeof(kEnum_EURC_state[0]), 0 },
    { 2, "position",       ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "calibration",    ::zhc::TuyaDpType::Enum, 1,
       kEnum_EURC_calibration, sizeof(kEnum_EURC_calibration)/sizeof(kEnum_EURC_calibration[0]), 0 },
    { 7, "backlight",      ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "motor_reversal", ::zhc::TuyaDpType::Enum, 1,
       kEnum_EURC_motor_reversal, sizeof(kEnum_EURC_motor_reversal)/sizeof(kEnum_EURC_motor_reversal[0]), 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_EURC{
    kEntries_EURC,
    sizeof(kEntries_EURC)/sizeof(kEntries_EURC[0])
};

constexpr FzConverter kFzDp_EURC{
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
    .user_config       = &kMap_EURC,
};
constexpr TzConverter kTzDp_EURC{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_EURC,
};

const FzConverter* const kFz_EURC[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_EURC,
};
const TzConverter* const kTz_EURC[] = { &kTzDp_EURC };

constexpr const char* kModels_EURC[] = { "TS0601" };
constexpr const char* kManus_EURC[] = {
    "_TZE200_nhyj64w2", "_TZE200_127x7wnl", "_TZE204_5slehgeo", "_TZE284_5slehgeo"
};

constexpr Expose kExp_EURC[] = {
    { "state",          ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "position",       ExposeType::Numeric, ::zhc::Access::StateSet, "%",     nullptr, nullptr, 0 },
    { "calibration",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "backlight",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "motor_reversal", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_EURC[] = { {1, 0xEF00} };
}  // namespace
extern const PreparedDefinition kDef_ZTS_EUR_C{
    .zigbee_models=kModels_EURC, .zigbee_models_count=sizeof(kModels_EURC)/sizeof(kModels_EURC[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_EURC, .manufacturer_names_count=sizeof(kManus_EURC)/sizeof(kManus_EURC[0]),
    .model="ZTS-EUR-C", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_EURC, .exposes_count=sizeof(kExp_EURC)/sizeof(kExp_EURC[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EURC, .from_zigbee_count=sizeof(kFz_EURC)/sizeof(kFz_EURC[0]),
    .to_zigbee=kTz_EURC, .to_zigbee_count=sizeof(kTz_EURC)/sizeof(kTz_EURC[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_EURC, .bindings_count=sizeof(kBind_EURC)/sizeof(kBind_EURC[0]),
};

}  // namespace zhc::devices::moes
