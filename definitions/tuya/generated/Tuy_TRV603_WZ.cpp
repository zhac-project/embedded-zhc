// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TRV603-WZ — auto-generated.
// Thermostatic radiator valve.
// z2m-source: tuya.ts #TRV603-WZ.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TRV603_WZ[] = { "TS0601" };
constexpr const char* kManus_TRV603_WZ[] = { "_TZE284_ymldrmzx" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TRV603_WZ_Tuy_TRV603_WZ[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TRV603_WZ_Tuy_TRV603_WZ[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TRV603_WZ_Tuy_TRV603_WZ[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TRV603_WZ{
    .zigbee_models=kModels_TRV603_WZ, .zigbee_models_count=sizeof(kModels_TRV603_WZ)/sizeof(kModels_TRV603_WZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TRV603_WZ, .manufacturer_names_count=sizeof(kManus_TRV603_WZ)/sizeof(kManus_TRV603_WZ[0]),
    .model="TRV603-WZ", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TRV603_WZ_Tuy_TRV603_WZ, .exposes_count=sizeof(kExp_min_TRV603_WZ_Tuy_TRV603_WZ)/sizeof(kExp_min_TRV603_WZ_Tuy_TRV603_WZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TRV603_WZ_Tuy_TRV603_WZ, .from_zigbee_count=sizeof(kFz_min_TRV603_WZ_Tuy_TRV603_WZ)/sizeof(kFz_min_TRV603_WZ_Tuy_TRV603_WZ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
