// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya WSD500A — auto-generated.
// Temperature & humidity sensor
// z2m-source: tuya.ts #WSD500A.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_WSD500A[] = { "TS0201" };
constexpr const char* kManus_WSD500A[] = { "_TZ3000_bguser20", "_TZ3000_yd2e749y", "_TZ3000_6uzkisv2", "_TZ3000_xr3htd96", "_TZ3000_fllyghyj", "_TZ3000_saiqcn0y", "_TZ3000_bjawzodf" };
}  // namespace

namespace {
const FzConverter* const kFz_min_WSD500A_Tuy_WSD500A[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_WSD500A_Tuy_WSD500A[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_WSD500A_Tuy_WSD500A[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_WSD500A{
    .zigbee_models=kModels_WSD500A, .zigbee_models_count=sizeof(kModels_WSD500A)/sizeof(kModels_WSD500A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_WSD500A, .manufacturer_names_count=sizeof(kManus_WSD500A)/sizeof(kManus_WSD500A[0]),
    .model="WSD500A", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_WSD500A_Tuy_WSD500A, .exposes_count=sizeof(kExp_min_WSD500A_Tuy_WSD500A)/sizeof(kExp_min_WSD500A_Tuy_WSD500A[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_WSD500A_Tuy_WSD500A, .from_zigbee_count=sizeof(kFz_min_WSD500A_Tuy_WSD500A)/sizeof(kFz_min_WSD500A_Tuy_WSD500A[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
