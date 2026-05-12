// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_thermostat_3 — auto-generated.
// Thermostatic radiator valve
// z2m-source: tuya.ts #TS0601_thermostat_3.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_thermostat_3[] = { "TS0601" };
constexpr const char* kManus_TS0601_thermostat_3[] = { "_TZE200_bvu2wnxz", "_TZE200_6rdj8dzm", "_TZE200_9xfjixap", "_TZE200_jkfbph7l", "_TZE200_rxntag7i", "_TZE200_4utwozi2", "_TZE200_yqgbrdyo", "_TZE284_p3dbf6qs", "_TZE200_rxq4iti9", "_TZE204_ogx8u5z6", "_TZE200_4utwoz2", "_TZE284_ogx8u5z6", "_TZE284_o3x45p96", "_TZE284_c6wv4xyo", "_TZE204_o3x45p96", "_TZE200_ow09xlxm", "_TZE284_rv6iuyxb", "_TZE200_rv6iuyxb", "_TZE200_suxywabt", "_TZE200_d3z1ukqw" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_thermostat_3{
    .zigbee_models=kModels_TS0601_thermostat_3, .zigbee_models_count=sizeof(kModels_TS0601_thermostat_3)/sizeof(kModels_TS0601_thermostat_3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_thermostat_3, .manufacturer_names_count=sizeof(kManus_TS0601_thermostat_3)/sizeof(kManus_TS0601_thermostat_3[0]),
    .model="TS0601_thermostat_3", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3, .exposes_count=sizeof(kExp_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3)/sizeof(kExp_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3, .from_zigbee_count=sizeof(kFz_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3)/sizeof(kFz_min_TS0601_thermostat_3_Tuy_TS0601_thermostat_3[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
