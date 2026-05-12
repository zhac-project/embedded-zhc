// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya CTL-R1-TY-Zigbee — auto-generated.
// 24G radar human presence motion sensor.
// z2m-source: tuya.ts #CTL-R1-TY-Zigbee.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_CTL_R1_TY_Zigbee[] = { "TS0601" };
constexpr const char* kManus_CTL_R1_TY_Zigbee[] = { "_TZE204_e9ajs4ft" };
}  // namespace

namespace {
const FzConverter* const kFz_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_CTL_R1_TY_Zigbee{
    .zigbee_models=kModels_CTL_R1_TY_Zigbee, .zigbee_models_count=sizeof(kModels_CTL_R1_TY_Zigbee)/sizeof(kModels_CTL_R1_TY_Zigbee[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_CTL_R1_TY_Zigbee, .manufacturer_names_count=sizeof(kManus_CTL_R1_TY_Zigbee)/sizeof(kManus_CTL_R1_TY_Zigbee[0]),
    .model="CTL-R1-TY-Zigbee", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee, .exposes_count=sizeof(kExp_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee)/sizeof(kExp_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee, .from_zigbee_count=sizeof(kFz_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee)/sizeof(kFz_min_CTL_R1_TY_Zigbee_Tuy_CTL_R1_TY_Zigbee[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
