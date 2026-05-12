// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya AE-940K — auto-generated.
// Compatible with Daikin vrv system
// z2m-source: tuya.ts #AE-940K.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_AE_940K[] = { "TS0601" };
constexpr const char* kManus_AE_940K[] = { "_TZE200_wem3gxyx" };
}  // namespace

namespace {
const FzConverter* const kFz_min_AE_940K_Tuy_AE_940K[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_AE_940K_Tuy_AE_940K[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_AE_940K_Tuy_AE_940K[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_AE_940K{
    .zigbee_models=kModels_AE_940K, .zigbee_models_count=sizeof(kModels_AE_940K)/sizeof(kModels_AE_940K[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_AE_940K, .manufacturer_names_count=sizeof(kManus_AE_940K)/sizeof(kManus_AE_940K[0]),
    .model="AE-940K", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_AE_940K_Tuy_AE_940K, .exposes_count=sizeof(kExp_min_AE_940K_Tuy_AE_940K)/sizeof(kExp_min_AE_940K_Tuy_AE_940K[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_AE_940K_Tuy_AE_940K, .from_zigbee_count=sizeof(kFz_min_AE_940K_Tuy_AE_940K)/sizeof(kFz_min_AE_940K_Tuy_AE_940K[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
