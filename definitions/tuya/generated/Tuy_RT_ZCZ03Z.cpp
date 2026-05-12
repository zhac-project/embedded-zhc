// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya RT_ZCZ03Z — auto-generated.
// Human presence sensor 24G
// z2m-source: tuya.ts #RT_ZCZ03Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_RT_ZCZ03Z[] = { "TS0601" };
constexpr const char* kManus_RT_ZCZ03Z[] = { "_TZE204_uxllnywp" };
}  // namespace

namespace {
const FzConverter* const kFz_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_RT_ZCZ03Z{
    .zigbee_models=kModels_RT_ZCZ03Z, .zigbee_models_count=sizeof(kModels_RT_ZCZ03Z)/sizeof(kModels_RT_ZCZ03Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_RT_ZCZ03Z, .manufacturer_names_count=sizeof(kManus_RT_ZCZ03Z)/sizeof(kManus_RT_ZCZ03Z[0]),
    .model="RT_ZCZ03Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z, .exposes_count=sizeof(kExp_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z)/sizeof(kExp_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z, .from_zigbee_count=sizeof(kFz_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z)/sizeof(kFz_min_RT_ZCZ03Z_Tuy_RT_ZCZ03Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
