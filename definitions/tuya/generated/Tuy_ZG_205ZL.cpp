// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-205ZL — auto-generated.
// 24Ghz/5.8GHz human presence sensor
// z2m-source: tuya.ts #ZG-205ZL.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_205ZL[] = { "ZG-205ZL", "TS0225", "CK-BL702-MWS-01(7016)" };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_205ZL_Tuy_ZG_205ZL[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_205ZL_Tuy_ZG_205ZL[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_205ZL_Tuy_ZG_205ZL[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_205ZL{
    .zigbee_models=kModels_ZG_205ZL, .zigbee_models_count=sizeof(kModels_ZG_205ZL)/sizeof(kModels_ZG_205ZL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-205ZL", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_205ZL_Tuy_ZG_205ZL, .exposes_count=sizeof(kExp_min_ZG_205ZL_Tuy_ZG_205ZL)/sizeof(kExp_min_ZG_205ZL_Tuy_ZG_205ZL[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_205ZL_Tuy_ZG_205ZL, .from_zigbee_count=sizeof(kFz_min_ZG_205ZL_Tuy_ZG_205ZL)/sizeof(kFz_min_ZG_205ZL_Tuy_ZG_205ZL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
