// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya L2-T-F-MF — auto-generated.
// Smart Zigbee fan coil thermostat
// z2m-source: tuya.ts #L2-T-F-MF.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_L2_T_F_MF[] = { "TS0601" };
constexpr const char* kManus_L2_T_F_MF[] = { "_TZE284_4vbj3fxh" };
}  // namespace

namespace {
const FzConverter* const kFz_min_L2_T_F_MF_Tuy_L2_T_F_MF[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_L2_T_F_MF_Tuy_L2_T_F_MF[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_L2_T_F_MF_Tuy_L2_T_F_MF[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_L2_T_F_MF{
    .zigbee_models=kModels_L2_T_F_MF, .zigbee_models_count=sizeof(kModels_L2_T_F_MF)/sizeof(kModels_L2_T_F_MF[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_L2_T_F_MF, .manufacturer_names_count=sizeof(kManus_L2_T_F_MF)/sizeof(kManus_L2_T_F_MF[0]),
    .model="L2-T-F-MF", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_L2_T_F_MF_Tuy_L2_T_F_MF, .exposes_count=sizeof(kExp_min_L2_T_F_MF_Tuy_L2_T_F_MF)/sizeof(kExp_min_L2_T_F_MF_Tuy_L2_T_F_MF[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_L2_T_F_MF_Tuy_L2_T_F_MF, .from_zigbee_count=sizeof(kFz_min_L2_T_F_MF_Tuy_L2_T_F_MF)/sizeof(kFz_min_L2_T_F_MF_Tuy_L2_T_F_MF[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
