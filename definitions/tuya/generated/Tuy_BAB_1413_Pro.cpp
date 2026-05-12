// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya BAB-1413_Pro — auto-generated.
// Thermostat radiator valve
// z2m-source: tuya.ts #BAB-1413_Pro.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_BAB_1413_Pro[] = { "TS0601" };
constexpr const char* kManus_BAB_1413_Pro[] = { "_TZE204_g2ki0ejr" };
}  // namespace

namespace {
const FzConverter* const kFz_min_BAB_1413_Pro_Tuy_BAB_1413_Pro[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_BAB_1413_Pro_Tuy_BAB_1413_Pro[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_BAB_1413_Pro_Tuy_BAB_1413_Pro[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_BAB_1413_Pro{
    .zigbee_models=kModels_BAB_1413_Pro, .zigbee_models_count=sizeof(kModels_BAB_1413_Pro)/sizeof(kModels_BAB_1413_Pro[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_BAB_1413_Pro, .manufacturer_names_count=sizeof(kManus_BAB_1413_Pro)/sizeof(kManus_BAB_1413_Pro[0]),
    .model="BAB-1413_Pro", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_BAB_1413_Pro_Tuy_BAB_1413_Pro, .exposes_count=sizeof(kExp_min_BAB_1413_Pro_Tuy_BAB_1413_Pro)/sizeof(kExp_min_BAB_1413_Pro_Tuy_BAB_1413_Pro[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_BAB_1413_Pro_Tuy_BAB_1413_Pro, .from_zigbee_count=sizeof(kFz_min_BAB_1413_Pro_Tuy_BAB_1413_Pro)/sizeof(kFz_min_BAB_1413_Pro_Tuy_BAB_1413_Pro[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
