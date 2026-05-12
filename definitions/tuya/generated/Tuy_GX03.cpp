// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya GX03 — auto-generated.
// GIEX 2 zone watering timer
// z2m-source: tuya.ts #GX03.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_GX03[] = { "TS0601" };
constexpr const char* kManus_GX03[] = { "_TZE284_8zizsafo", "_TZE284_iilebqoo" };
}  // namespace

namespace {
const FzConverter* const kFz_min_GX03_Tuy_GX03[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_GX03_Tuy_GX03[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_GX03_Tuy_GX03[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_GX03{
    .zigbee_models=kModels_GX03, .zigbee_models_count=sizeof(kModels_GX03)/sizeof(kModels_GX03[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_GX03, .manufacturer_names_count=sizeof(kManus_GX03)/sizeof(kManus_GX03[0]),
    .model="GX03", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_GX03_Tuy_GX03, .exposes_count=sizeof(kExp_min_GX03_Tuy_GX03)/sizeof(kExp_min_GX03_Tuy_GX03[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_GX03_Tuy_GX03, .from_zigbee_count=sizeof(kFz_min_GX03_Tuy_GX03)/sizeof(kFz_min_GX03_Tuy_GX03[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
