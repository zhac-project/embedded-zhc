// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_soil — auto-generated.
// Soil sensor
// z2m-source: tuya.ts #TS0601_soil.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_soil[] = { "TS0601" };
constexpr const char* kManus_TS0601_soil[] = { "_TZE200_myd45weu", "_TZE200_ga1maeof", "_TZE200_2se8efxh", "_TZE204_myd45weu", "_TZE284_myd45weu", "_TZE284_oitavov2", "_TZE284_2se8efxh", "_TZE200_9cqcpkgb" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_soil_Tuy_TS0601_soil[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_soil_Tuy_TS0601_soil[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_soil_Tuy_TS0601_soil[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_soil{
    .zigbee_models=kModels_TS0601_soil, .zigbee_models_count=sizeof(kModels_TS0601_soil)/sizeof(kModels_TS0601_soil[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_soil, .manufacturer_names_count=sizeof(kManus_TS0601_soil)/sizeof(kManus_TS0601_soil[0]),
    .model="TS0601_soil", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_soil_Tuy_TS0601_soil, .exposes_count=sizeof(kExp_min_TS0601_soil_Tuy_TS0601_soil)/sizeof(kExp_min_TS0601_soil_Tuy_TS0601_soil[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_soil_Tuy_TS0601_soil, .from_zigbee_count=sizeof(kFz_min_TS0601_soil_Tuy_TS0601_soil)/sizeof(kFz_min_TS0601_soil_Tuy_TS0601_soil[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
