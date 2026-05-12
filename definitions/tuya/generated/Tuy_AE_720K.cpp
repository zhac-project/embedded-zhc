// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya AE-720K — auto-generated.
// Compatible with Daikin vrv system
// z2m-source: tuya.ts #AE-720K.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_AE_720K[] = { "TS0601" };
constexpr const char* kManus_AE_720K[] = { "_TZE284_mul9abs3" };
}  // namespace

namespace {
const FzConverter* const kFz_min_AE_720K_Tuy_AE_720K[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_AE_720K_Tuy_AE_720K[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_AE_720K_Tuy_AE_720K[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_AE_720K{
    .zigbee_models=kModels_AE_720K, .zigbee_models_count=sizeof(kModels_AE_720K)/sizeof(kModels_AE_720K[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_AE_720K, .manufacturer_names_count=sizeof(kManus_AE_720K)/sizeof(kManus_AE_720K[0]),
    .model="AE-720K", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_AE_720K_Tuy_AE_720K, .exposes_count=sizeof(kExp_min_AE_720K_Tuy_AE_720K)/sizeof(kExp_min_AE_720K_Tuy_AE_720K[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_AE_720K_Tuy_AE_720K, .from_zigbee_count=sizeof(kFz_min_AE_720K_Tuy_AE_720K)/sizeof(kFz_min_AE_720K_Tuy_AE_720K[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
