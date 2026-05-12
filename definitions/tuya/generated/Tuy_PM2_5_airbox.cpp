// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya PM2.5_airbox — auto-generated.
// PM2.5 air quality sensor
// z2m-source: tuya.ts #PM2.5_airbox.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_PM2_5_airbox[] = { "TS0601" };
constexpr const char* kManus_PM2_5_airbox[] = { "_TZE284_it9utkro" };
}  // namespace

namespace {
const FzConverter* const kFz_min_PM2_5_airbox_Tuy_PM2_5_airbox[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_PM2_5_airbox_Tuy_PM2_5_airbox[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PM2_5_airbox_Tuy_PM2_5_airbox[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_PM2_5_airbox{
    .zigbee_models=kModels_PM2_5_airbox, .zigbee_models_count=sizeof(kModels_PM2_5_airbox)/sizeof(kModels_PM2_5_airbox[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_PM2_5_airbox, .manufacturer_names_count=sizeof(kManus_PM2_5_airbox)/sizeof(kManus_PM2_5_airbox[0]),
    .model="PM2.5_airbox", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_PM2_5_airbox_Tuy_PM2_5_airbox, .exposes_count=sizeof(kExp_min_PM2_5_airbox_Tuy_PM2_5_airbox)/sizeof(kExp_min_PM2_5_airbox_Tuy_PM2_5_airbox[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PM2_5_airbox_Tuy_PM2_5_airbox, .from_zigbee_count=sizeof(kFz_min_PM2_5_airbox_Tuy_PM2_5_airbox)/sizeof(kFz_min_PM2_5_airbox_Tuy_PM2_5_airbox[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
