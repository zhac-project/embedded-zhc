// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Moes ZTS-EU_2gang — auto-generated.
// Wall touch light switch (2 gang)
// z2m-source: moes.ts #ZTS-EU_2gang.
// Phase 2 (2026-04-27): wired Tuya DP map from legacy.fz.tuya_switch +
// legacy.fz.moes_switch (legacy.ts:3592-3614).
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZTS_EU_2gang_pob[] = {
    { 0, "off" }, { 1, "on" }, { 2, "previous" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZTS_EU_2gang_il[] = {
    { 0, "off" }, { 1, "switch" }, { 2, "position" }, { 3, "freeze" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_ZTS_EU_2gang[] = {
    { 1,  "state_l1",                       ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2,  "state_l2",                       ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "power_on_behavior", ::zhc::TuyaDpType::Enum, 1,
       kEnum_ZTS_EU_2gang_pob, sizeof(kEnum_ZTS_EU_2gang_pob)/sizeof(kEnum_ZTS_EU_2gang_pob[0]), 0 },
    { 15, "indicate_light",    ::zhc::TuyaDpType::Enum, 1,
       kEnum_ZTS_EU_2gang_il, sizeof(kEnum_ZTS_EU_2gang_il)/sizeof(kEnum_ZTS_EU_2gang_il[0]), 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_ZTS_EU_2gang{
    kEntries_ZTS_EU_2gang,
    sizeof(kEntries_ZTS_EU_2gang)/sizeof(kEntries_ZTS_EU_2gang[0])
};

constexpr FzConverter kFzDp_ZTS_EU_2gang{
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
    .user_config       = &kMap_ZTS_EU_2gang,
};
constexpr TzConverter kTzDp_ZTS_EU_2gang{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_ZTS_EU_2gang,
};

const FzConverter* const kFz_ZTS_EU_2gang[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ZTS_EU_2gang,
};
const TzConverter* const kTz_ZTS_EU_2gang[] = { &kTzDp_ZTS_EU_2gang };

constexpr const char* kModels_ZTS_EU_2gang[] = { "TS0601" };
constexpr const char* kManus_ZTS_EU_2gang[] = { "_TZE200_g1ib5ldv" };

constexpr Expose kExp_ZTS_EU_2gang[] = {
    { "state_l1",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l2",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "power_on_behavior", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "indicate_light",    ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_ZTS_EU_2gang[] = { {1, 0xEF00} };
}  // namespace
extern const PreparedDefinition kDef_ZTS_EU_2gang{
    .zigbee_models=kModels_ZTS_EU_2gang, .zigbee_models_count=sizeof(kModels_ZTS_EU_2gang)/sizeof(kModels_ZTS_EU_2gang[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZTS_EU_2gang, .manufacturer_names_count=sizeof(kManus_ZTS_EU_2gang)/sizeof(kManus_ZTS_EU_2gang[0]),
    .model="ZTS-EU_2gang", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_ZTS_EU_2gang, .exposes_count=sizeof(kExp_ZTS_EU_2gang)/sizeof(kExp_ZTS_EU_2gang[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZTS_EU_2gang, .from_zigbee_count=sizeof(kFz_ZTS_EU_2gang)/sizeof(kFz_ZTS_EU_2gang[0]),
    .to_zigbee=kTz_ZTS_EU_2gang, .to_zigbee_count=sizeof(kTz_ZTS_EU_2gang)/sizeof(kTz_ZTS_EU_2gang[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_ZTS_EU_2gang, .bindings_count=sizeof(kBind_ZTS_EU_2gang)/sizeof(kBind_ZTS_EU_2gang[0]),
};

}  // namespace zhc::devices::moes
