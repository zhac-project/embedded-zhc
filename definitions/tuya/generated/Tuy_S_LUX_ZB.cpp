// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya S-LUX-ZB — auto-generated.
// Light sensor
// z2m-source: tuya.ts #S-LUX-ZB.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_S_LUX_ZB[] = { "isltm67\\u0000", "TS0601" };
constexpr const char* kManus_S_LUX_ZB[] = { "_TYST11_pisltm67", "_TZE200_pisltm67" };
}  // namespace

namespace {
const FzConverter* const kFz_min_S_LUX_ZB_Tuy_S_LUX_ZB[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_S_LUX_ZB_Tuy_S_LUX_ZB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_S_LUX_ZB_Tuy_S_LUX_ZB[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_S_LUX_ZB{
    .zigbee_models=kModels_S_LUX_ZB, .zigbee_models_count=sizeof(kModels_S_LUX_ZB)/sizeof(kModels_S_LUX_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_S_LUX_ZB, .manufacturer_names_count=sizeof(kManus_S_LUX_ZB)/sizeof(kManus_S_LUX_ZB[0]),
    .model="S-LUX-ZB", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_S_LUX_ZB_Tuy_S_LUX_ZB, .exposes_count=sizeof(kExp_min_S_LUX_ZB_Tuy_S_LUX_ZB)/sizeof(kExp_min_S_LUX_ZB_Tuy_S_LUX_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_S_LUX_ZB_Tuy_S_LUX_ZB, .from_zigbee_count=sizeof(kFz_min_S_LUX_ZB_Tuy_S_LUX_ZB)/sizeof(kFz_min_S_LUX_ZB_Tuy_S_LUX_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
