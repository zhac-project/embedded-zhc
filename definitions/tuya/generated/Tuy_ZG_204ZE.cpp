// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-204ZE — auto-generated.
// 10G mw motion detection
// z2m-source: tuya.ts #ZG-204ZE.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_204ZE[] = { "ZG-204ZE", "CK-BL702-MWS-01(7016)", "TS0601" };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_204ZE_Tuy_ZG_204ZE[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_204ZE_Tuy_ZG_204ZE[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_204ZE_Tuy_ZG_204ZE[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_204ZE{
    .zigbee_models=kModels_ZG_204ZE, .zigbee_models_count=sizeof(kModels_ZG_204ZE)/sizeof(kModels_ZG_204ZE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-204ZE", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_204ZE_Tuy_ZG_204ZE, .exposes_count=sizeof(kExp_min_ZG_204ZE_Tuy_ZG_204ZE)/sizeof(kExp_min_ZG_204ZE_Tuy_ZG_204ZE[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_204ZE_Tuy_ZG_204ZE, .from_zigbee_count=sizeof(kFz_min_ZG_204ZE_Tuy_ZG_204ZE)/sizeof(kFz_min_ZG_204ZE_Tuy_ZG_204ZE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
