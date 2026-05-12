// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya YXZBSL — auto-generated.
// Smart siren
// z2m-source: tuya.ts #YXZBSL.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_YXZBSL[] = { "TS0601" };
constexpr const char* kManus_YXZBSL[] = { "_TZE204_k7mfgaen", "_TZE204_fncxk3ob", "_TZE284_fncxk3ob" };
}  // namespace

namespace {
const FzConverter* const kFz_min_YXZBSL_Tuy_YXZBSL[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_YXZBSL_Tuy_YXZBSL[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_YXZBSL_Tuy_YXZBSL[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_YXZBSL{
    .zigbee_models=kModels_YXZBSL, .zigbee_models_count=sizeof(kModels_YXZBSL)/sizeof(kModels_YXZBSL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_YXZBSL, .manufacturer_names_count=sizeof(kManus_YXZBSL)/sizeof(kManus_YXZBSL[0]),
    .model="YXZBSL", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_YXZBSL_Tuy_YXZBSL, .exposes_count=sizeof(kExp_min_YXZBSL_Tuy_YXZBSL)/sizeof(kExp_min_YXZBSL_Tuy_YXZBSL[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_YXZBSL_Tuy_YXZBSL, .from_zigbee_count=sizeof(kFz_min_YXZBSL_Tuy_YXZBSL)/sizeof(kFz_min_YXZBSL_Tuy_YXZBSL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
