// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_smart_air_house_keeper — auto-generated.
// Smart air house keeper
// z2m-source: tuya.ts #TS0601_smart_air_house_keeper.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_smart_air_house_keeper[] = { "TS0601" };
constexpr const char* kManus_TS0601_smart_air_house_keeper[] = { "_TZE200_dwcarsat", "_TZE204_dwcarsat", "_TZE200_blfcpsxz" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_smart_air_house_keeper{
    .zigbee_models=kModels_TS0601_smart_air_house_keeper, .zigbee_models_count=sizeof(kModels_TS0601_smart_air_house_keeper)/sizeof(kModels_TS0601_smart_air_house_keeper[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_smart_air_house_keeper, .manufacturer_names_count=sizeof(kManus_TS0601_smart_air_house_keeper)/sizeof(kManus_TS0601_smart_air_house_keeper[0]),
    .model="TS0601_smart_air_house_keeper", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee, .exposes_count=sizeof(kExp_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee)/sizeof(kExp_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee, .from_zigbee_count=sizeof(kFz_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee)/sizeof(kFz_min_TS0601_smart_air_house_keeper_Tuy_TS0601_smart_air_house_kee[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
