// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TGM50-ZB — auto-generated.
// Beok wall thermostat
// z2m-source: tuya.ts #TGM50-ZB.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TGM50_ZB[] = { "TS0601" };
constexpr const char* kManus_TGM50_ZB[] = { "_TZE204_mwomyz5n", "_TZE204_cvub6xbb", "_TZE284_cvub6xbb" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TGM50_ZB_Tuy_TGM50_ZB[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TGM50_ZB_Tuy_TGM50_ZB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TGM50_ZB_Tuy_TGM50_ZB[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TGM50_ZB{
    .zigbee_models=kModels_TGM50_ZB, .zigbee_models_count=sizeof(kModels_TGM50_ZB)/sizeof(kModels_TGM50_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TGM50_ZB, .manufacturer_names_count=sizeof(kManus_TGM50_ZB)/sizeof(kManus_TGM50_ZB[0]),
    .model="TGM50-ZB", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TGM50_ZB_Tuy_TGM50_ZB, .exposes_count=sizeof(kExp_min_TGM50_ZB_Tuy_TGM50_ZB)/sizeof(kExp_min_TGM50_ZB_Tuy_TGM50_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TGM50_ZB_Tuy_TGM50_ZB, .from_zigbee_count=sizeof(kFz_min_TGM50_ZB_Tuy_TGM50_ZB)/sizeof(kFz_min_TGM50_ZB_Tuy_TGM50_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
