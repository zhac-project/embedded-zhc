// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-204ZM — auto-generated.
// PIR 24Ghz human presence sensor
// z2m-source: tuya.ts #ZG-204ZM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_204ZM[] = { "ZG-204ZM", "TS0601" };
constexpr const char* kManus_ZG_204ZM[] = { "_TZE200_2aaelwxk", "_TZE200_kb5noeto", "_TZE200_tyffvoij", "_TZE200_yflzeeqj" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_204ZM_Tuy_ZG_204ZM[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_204ZM_Tuy_ZG_204ZM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_204ZM_Tuy_ZG_204ZM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_204ZM{
    .zigbee_models=kModels_ZG_204ZM, .zigbee_models_count=sizeof(kModels_ZG_204ZM)/sizeof(kModels_ZG_204ZM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZG_204ZM, .manufacturer_names_count=sizeof(kManus_ZG_204ZM)/sizeof(kManus_ZG_204ZM[0]),
    .model="ZG-204ZM", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_204ZM_Tuy_ZG_204ZM, .exposes_count=sizeof(kExp_min_ZG_204ZM_Tuy_ZG_204ZM)/sizeof(kExp_min_ZG_204ZM_Tuy_ZG_204ZM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_204ZM_Tuy_ZG_204ZM, .from_zigbee_count=sizeof(kFz_min_ZG_204ZM_Tuy_ZG_204ZM)/sizeof(kFz_min_ZG_204ZM_Tuy_ZG_204ZM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
