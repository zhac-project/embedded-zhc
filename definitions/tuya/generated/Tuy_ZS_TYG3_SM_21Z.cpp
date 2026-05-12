// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZS-TYG3-SM-21Z — auto-generated.
// 2 gang smart switch with backlight and neutral wire
// z2m-source: tuya.ts #ZS-TYG3-SM-21Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZS_TYG3_SM_21Z[] = { "TS0601" };
constexpr const char* kManus_ZS_TYG3_SM_21Z[] = { "_TZE200_wunufsil" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZS_TYG3_SM_21Z{
    .zigbee_models=kModels_ZS_TYG3_SM_21Z, .zigbee_models_count=sizeof(kModels_ZS_TYG3_SM_21Z)/sizeof(kModels_ZS_TYG3_SM_21Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZS_TYG3_SM_21Z, .manufacturer_names_count=sizeof(kManus_ZS_TYG3_SM_21Z)/sizeof(kManus_ZS_TYG3_SM_21Z[0]),
    .model="ZS-TYG3-SM-21Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z, .exposes_count=sizeof(kExp_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z)/sizeof(kExp_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z, .from_zigbee_count=sizeof(kFz_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z)/sizeof(kFz_min_ZS_TYG3_SM_21Z_Tuy_ZS_TYG3_SM_21Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
