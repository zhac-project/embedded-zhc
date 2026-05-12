// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-302ZM — auto-generated.
// Motion sensing switch
// z2m-source: tuya.ts #ZG-302ZM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_302ZM[] = { "ZG-302ZM", "TS0601" };
constexpr const char* kManus_ZG_302ZM[] = { "_TZE200_kccdzaeo", "_TZE200_s7rsrtbg", "_TZE200_tmszbtzq", "_TZE200_bfmfhxra", "_TZE200_ahpcyzth", "_TZE200_kijxnb8q" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_302ZM_Tuy_ZG_302ZM[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_302ZM_Tuy_ZG_302ZM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_302ZM_Tuy_ZG_302ZM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_302ZM{
    .zigbee_models=kModels_ZG_302ZM, .zigbee_models_count=sizeof(kModels_ZG_302ZM)/sizeof(kModels_ZG_302ZM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZG_302ZM, .manufacturer_names_count=sizeof(kManus_ZG_302ZM)/sizeof(kManus_ZG_302ZM[0]),
    .model="ZG-302ZM", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_302ZM_Tuy_ZG_302ZM, .exposes_count=sizeof(kExp_min_ZG_302ZM_Tuy_ZG_302ZM)/sizeof(kExp_min_ZG_302ZM_Tuy_ZG_302ZM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_302ZM_Tuy_ZG_302ZM, .from_zigbee_count=sizeof(kFz_min_ZG_302ZM_Tuy_ZG_302ZM)/sizeof(kFz_min_ZG_302ZM_Tuy_ZG_302ZM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
