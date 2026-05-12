// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya rtsc11r — auto-generated.
// 5.8G human presence sensor with relay
// z2m-source: tuya.ts #rtsc11r.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_rtsc11r[] = { "TS0601" };
constexpr const char* kManus_rtsc11r[] = { "_TZE204_mhxn2jso" };
}  // namespace

namespace {
const FzConverter* const kFz_min_rtsc11r_Tuy_rtsc11r[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_rtsc11r_Tuy_rtsc11r[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_rtsc11r_Tuy_rtsc11r[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_rtsc11r{
    .zigbee_models=kModels_rtsc11r, .zigbee_models_count=sizeof(kModels_rtsc11r)/sizeof(kModels_rtsc11r[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_rtsc11r, .manufacturer_names_count=sizeof(kManus_rtsc11r)/sizeof(kManus_rtsc11r[0]),
    .model="rtsc11r", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_rtsc11r_Tuy_rtsc11r, .exposes_count=sizeof(kExp_min_rtsc11r_Tuy_rtsc11r)/sizeof(kExp_min_rtsc11r_Tuy_rtsc11r[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_rtsc11r_Tuy_rtsc11r, .from_zigbee_count=sizeof(kFz_min_rtsc11r_Tuy_rtsc11r)/sizeof(kFz_min_rtsc11r_Tuy_rtsc11r[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
