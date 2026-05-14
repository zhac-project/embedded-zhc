// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/moes.ts (fingerprint TS0601 / _TZE200_1ozguk6x).
// Phase 2 (2026-04-27): wired Tuya DP map from legacy.fz.tuya_switch +
// legacy.fz.moes_switch (legacy.ts:3592-3614). DPs:
//   1..4 state per endpoint l1..l4
//   14 powerOnBehavior Enum (0=off/1=on/2=previous)
//   15 indicateLight Enum (0=off/1=switch/2=position/3=freeze)
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZTS_EU_power_on_behavior[] = {
    { 0, "off" }, { 1, "on" }, { 2, "previous" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZTS_EU_indicate_light[] = {
    { 0, "off" }, { 1, "switch" }, { 2, "position" }, { 3, "freeze" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_ZTS_EU[] = {
    { 1,  "state_l1",                       ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2,  "state_l2",                       ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3,  "state_l3",                       ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4,  "state_l4",                       ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1,
       kEnum_ZTS_EU_power_on_behavior, sizeof(kEnum_ZTS_EU_power_on_behavior)/sizeof(kEnum_ZTS_EU_power_on_behavior[0]), 0 },
    { 15, "indicate_light",    ::zhc::TuyaDpType::Enum, 1,
       kEnum_ZTS_EU_indicate_light,    sizeof(kEnum_ZTS_EU_indicate_light)/sizeof(kEnum_ZTS_EU_indicate_light[0]), 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_ZTS_EU{
    kEntries_ZTS_EU,
    sizeof(kEntries_ZTS_EU)/sizeof(kEntries_ZTS_EU[0])
};

constexpr FzConverter kFzDp_ZTS_EU{
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
    .user_config       = &kMap_ZTS_EU,
};
constexpr TzConverter kTzDp_ZTS_EU{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_ZTS_EU,
};

const FzConverter* const kFz_ZTS_EU[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ZTS_EU,
};
const TzConverter* const kTz_ZTS_EU[] = { &kTzDp_ZTS_EU };

constexpr const char* kM_ZTS_EU[] = { "TS0601" };
constexpr const char* kN_ZTS_EU[] = { "_TZE200_1ozguk6x" };

constexpr Expose kExp_ZTS_EU[] = {
    { "state_l1",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l2",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l3",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l4",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "power_on_behavior", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "indicate_light",    ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBindings_ZTS_EU[] = { {1, 0xEF00} };
}  // namespace
extern const PreparedDefinition kDefMoes__TZE200_1ozguk6x{
    .zigbee_models=kM_ZTS_EU,.zigbee_models_count=sizeof(kM_ZTS_EU)/sizeof(kM_ZTS_EU[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_ZTS_EU, .manufacturer_names_count=1,
    .model="TS0601__TZE200_1ozguk6x", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_ZTS_EU, .exposes_count=sizeof(kExp_ZTS_EU)/sizeof(kExp_ZTS_EU[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZTS_EU, .from_zigbee_count=sizeof(kFz_ZTS_EU)/sizeof(kFz_ZTS_EU[0]),
    .to_zigbee=kTz_ZTS_EU, .to_zigbee_count=sizeof(kTz_ZTS_EU)/sizeof(kTz_ZTS_EU[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBindings_ZTS_EU, .bindings_count=sizeof(kBindings_ZTS_EU)/sizeof(kBindings_ZTS_EU[0]),
};
}  // namespace zhc::devices::moes
