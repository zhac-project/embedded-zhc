// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya HY08WE — auto-generated.
// Wall-mount thermostat
// z2m-source: tuya.ts #HY08WE.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_HY08WE[] = { "TS0601" };
constexpr const char* kManus_HY08WE[] = { "_TZE200_znzs7yaw" };
}  // namespace

namespace {
const FzConverter* const kFz_min_HY08WE_Tuy_HY08WE[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_HY08WE_Tuy_HY08WE[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_HY08WE_Tuy_HY08WE[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_HY08WE{
    .zigbee_models=kModels_HY08WE, .zigbee_models_count=sizeof(kModels_HY08WE)/sizeof(kModels_HY08WE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_HY08WE, .manufacturer_names_count=sizeof(kManus_HY08WE)/sizeof(kManus_HY08WE[0]),
    .model="HY08WE", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_HY08WE_Tuy_HY08WE, .exposes_count=sizeof(kExp_min_HY08WE_Tuy_HY08WE)/sizeof(kExp_min_HY08WE_Tuy_HY08WE[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_HY08WE_Tuy_HY08WE, .from_zigbee_count=sizeof(kFz_min_HY08WE_Tuy_HY08WE)/sizeof(kFz_min_HY08WE_Tuy_HY08WE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
