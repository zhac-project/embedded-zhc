// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya MTG075-ZB-RL — auto-generated.
// 2.4G/5.8G human presence sensor with relay
// z2m-source: tuya.ts #MTG075-ZB-RL.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_MTG075_ZB_RL[] = { "TS0601" };
constexpr const char* kManus_MTG075_ZB_RL[] = { "_TZE204_sbyx0lm6", "_TZE204_clrdrnya", "_TZE204_dtzziy1e", "_TZE204_iaeejhvf", "_TZE204_mtoaryre", "_TZE200_mp902om5", "_TZE204_pfayrzcw", "_TZE284_4qznlkbu", "_TZE200_clrdrnya", "_TZE200_sbyx0lm6" };
}  // namespace

namespace {
const FzConverter* const kFz_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_MTG075_ZB_RL{
    .zigbee_models=kModels_MTG075_ZB_RL, .zigbee_models_count=sizeof(kModels_MTG075_ZB_RL)/sizeof(kModels_MTG075_ZB_RL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_MTG075_ZB_RL, .manufacturer_names_count=sizeof(kManus_MTG075_ZB_RL)/sizeof(kManus_MTG075_ZB_RL[0]),
    .model="MTG075-ZB-RL", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL, .exposes_count=sizeof(kExp_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL)/sizeof(kExp_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL, .from_zigbee_count=sizeof(kFz_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL)/sizeof(kFz_min_MTG075_ZB_RL_Tuy_MTG075_ZB_RL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
