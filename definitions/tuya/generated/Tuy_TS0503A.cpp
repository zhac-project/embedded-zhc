// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0503A — auto-generated.
// Led strip controller
// z2m-source: tuya.ts #TS0503A.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0503A[] = { "TS0503A" };
constexpr const char* kManus_TS0503A[] = { "_TZ3000_obacbukl" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0503A_Tuy_TS0503A[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0503A_Tuy_TS0503A[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0503A_Tuy_TS0503A[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0503A{
    .zigbee_models=kModels_TS0503A, .zigbee_models_count=sizeof(kModels_TS0503A)/sizeof(kModels_TS0503A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0503A, .manufacturer_names_count=sizeof(kManus_TS0503A)/sizeof(kManus_TS0503A[0]),
    .model="TS0503A", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0503A_Tuy_TS0503A, .exposes_count=sizeof(kExp_min_TS0503A_Tuy_TS0503A)/sizeof(kExp_min_TS0503A_Tuy_TS0503A[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0503A_Tuy_TS0503A, .from_zigbee_count=sizeof(kFz_min_TS0503A_Tuy_TS0503A)/sizeof(kFz_min_TS0503A_Tuy_TS0503A[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
