// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_dimmer_4 — auto-generated.
// 2 gang smart dimmer module
// z2m-source: tuya.ts #TS0601_dimmer_4.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_dimmer_4[] = { "TS0601" };
constexpr const char* kManus_TS0601_dimmer_4[] = { "_TZE200_bxoo2swd", "_TZE204_bxoo2swd", "_TZE200_tsxpl0d0" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_dimmer_4{
    .zigbee_models=kModels_TS0601_dimmer_4, .zigbee_models_count=sizeof(kModels_TS0601_dimmer_4)/sizeof(kModels_TS0601_dimmer_4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_dimmer_4, .manufacturer_names_count=sizeof(kManus_TS0601_dimmer_4)/sizeof(kManus_TS0601_dimmer_4[0]),
    .model="TS0601_dimmer_4", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4, .exposes_count=sizeof(kExp_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4)/sizeof(kExp_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4, .from_zigbee_count=sizeof(kFz_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4)/sizeof(kFz_min_TS0601_dimmer_4_Tuy_TS0601_dimmer_4[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
