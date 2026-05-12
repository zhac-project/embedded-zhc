// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya SA12IZL — auto-generated.
// Smart smoke alarm
// z2m-source: tuya.ts #SA12IZL.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_SA12IZL[] = { "TS0601" };
constexpr const char* kManus_SA12IZL[] = { "_TZE200_5d3vhjro" };
}  // namespace

namespace {
const FzConverter* const kFz_min_SA12IZL_Tuy_SA12IZL[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SA12IZL_Tuy_SA12IZL[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SA12IZL_Tuy_SA12IZL[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_SA12IZL{
    .zigbee_models=kModels_SA12IZL, .zigbee_models_count=sizeof(kModels_SA12IZL)/sizeof(kModels_SA12IZL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SA12IZL, .manufacturer_names_count=sizeof(kManus_SA12IZL)/sizeof(kManus_SA12IZL[0]),
    .model="SA12IZL", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_SA12IZL_Tuy_SA12IZL, .exposes_count=sizeof(kExp_min_SA12IZL_Tuy_SA12IZL)/sizeof(kExp_min_SA12IZL_Tuy_SA12IZL[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SA12IZL_Tuy_SA12IZL, .from_zigbee_count=sizeof(kFz_min_SA12IZL_Tuy_SA12IZL)/sizeof(kFz_min_SA12IZL_Tuy_SA12IZL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
