// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya Y1_IN — auto-generated.
// Smart human presence sensor 24G
// z2m-source: tuya.ts #Y1_IN.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_Y1_IN[] = { "TS0601" };
constexpr const char* kManus_Y1_IN[] = { "_TZE204_bmdsp6bs" };
}  // namespace

namespace {
const FzConverter* const kFz_min_Y1_IN_Tuy_Y1_IN[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_Y1_IN_Tuy_Y1_IN[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_Y1_IN_Tuy_Y1_IN[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_Y1_IN{
    .zigbee_models=kModels_Y1_IN, .zigbee_models_count=sizeof(kModels_Y1_IN)/sizeof(kModels_Y1_IN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_Y1_IN, .manufacturer_names_count=sizeof(kManus_Y1_IN)/sizeof(kManus_Y1_IN[0]),
    .model="Y1_IN", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_Y1_IN_Tuy_Y1_IN, .exposes_count=sizeof(kExp_min_Y1_IN_Tuy_Y1_IN)/sizeof(kExp_min_Y1_IN_Tuy_Y1_IN[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_Y1_IN_Tuy_Y1_IN, .from_zigbee_count=sizeof(kFz_min_Y1_IN_Tuy_Y1_IN)/sizeof(kFz_min_Y1_IN_Tuy_Y1_IN[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
