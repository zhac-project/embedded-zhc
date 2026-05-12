// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya PJ3201A — auto-generated.
// Human Presence Sensor
// z2m-source: tuya.ts #PJ3201A.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_PJ3201A[] = { "TS0601" };
constexpr const char* kManus_PJ3201A[] = { "_TZE204_eaulras5" };
}  // namespace

namespace {
const FzConverter* const kFz_min_PJ3201A_Tuy_PJ3201A[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_PJ3201A_Tuy_PJ3201A[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PJ3201A_Tuy_PJ3201A[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_PJ3201A{
    .zigbee_models=kModels_PJ3201A, .zigbee_models_count=sizeof(kModels_PJ3201A)/sizeof(kModels_PJ3201A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_PJ3201A, .manufacturer_names_count=sizeof(kManus_PJ3201A)/sizeof(kManus_PJ3201A[0]),
    .model="PJ3201A", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_PJ3201A_Tuy_PJ3201A, .exposes_count=sizeof(kExp_min_PJ3201A_Tuy_PJ3201A)/sizeof(kExp_min_PJ3201A_Tuy_PJ3201A[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PJ3201A_Tuy_PJ3201A, .from_zigbee_count=sizeof(kFz_min_PJ3201A_Tuy_PJ3201A)/sizeof(kFz_min_PJ3201A_Tuy_PJ3201A[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
