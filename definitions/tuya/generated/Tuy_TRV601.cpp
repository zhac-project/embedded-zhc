// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TRV601 — auto-generated.
// Thermostatic radiator valve
// z2m-source: tuya.ts #TRV601.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TRV601[] = { "TS0601" };
constexpr const char* kManus_TRV601[] = { "_TZE204_rtrmfadk", "_TZE204_cvcu2p6e" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TRV601_Tuy_TRV601[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TRV601_Tuy_TRV601[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TRV601_Tuy_TRV601[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TRV601{
    .zigbee_models=kModels_TRV601, .zigbee_models_count=sizeof(kModels_TRV601)/sizeof(kModels_TRV601[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TRV601, .manufacturer_names_count=sizeof(kManus_TRV601)/sizeof(kManus_TRV601[0]),
    .model="TRV601", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TRV601_Tuy_TRV601, .exposes_count=sizeof(kExp_min_TRV601_Tuy_TRV601)/sizeof(kExp_min_TRV601_Tuy_TRV601[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TRV601_Tuy_TRV601, .from_zigbee_count=sizeof(kFz_min_TRV601_Tuy_TRV601)/sizeof(kFz_min_TRV601_Tuy_TRV601[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
