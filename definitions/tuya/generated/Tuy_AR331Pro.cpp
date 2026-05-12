// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya AR331Pro — auto-generated.
// Thermostatic radiator valve
// z2m-source: tuya.ts #AR331Pro.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_AR331Pro[] = { "TS0601" };
constexpr const char* kManus_AR331Pro[] = { "_TZE284_nbv4tdaz" };
}  // namespace

namespace {
const FzConverter* const kFz_min_AR331Pro_Tuy_AR331Pro[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_AR331Pro_Tuy_AR331Pro[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_AR331Pro_Tuy_AR331Pro[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_AR331Pro{
    .zigbee_models=kModels_AR331Pro, .zigbee_models_count=sizeof(kModels_AR331Pro)/sizeof(kModels_AR331Pro[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_AR331Pro, .manufacturer_names_count=sizeof(kManus_AR331Pro)/sizeof(kManus_AR331Pro[0]),
    .model="AR331Pro", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_AR331Pro_Tuy_AR331Pro, .exposes_count=sizeof(kExp_min_AR331Pro_Tuy_AR331Pro)/sizeof(kExp_min_AR331Pro_Tuy_AR331Pro[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_AR331Pro_Tuy_AR331Pro, .from_zigbee_count=sizeof(kFz_min_AR331Pro_Tuy_AR331Pro)/sizeof(kFz_min_AR331Pro_Tuy_AR331Pro[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
