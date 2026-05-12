// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya S8 — auto-generated.
// S8 premium window handle
// z2m-source: tuya.ts #S8.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_S8[] = { "TS0601" };
constexpr const char* kManus_S8[] = { "_TZE200_j7sgd8po" };
}  // namespace

namespace {
const FzConverter* const kFz_min_S8_Tuy_S8[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_S8_Tuy_S8[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_S8_Tuy_S8[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_S8{
    .zigbee_models=kModels_S8, .zigbee_models_count=sizeof(kModels_S8)/sizeof(kModels_S8[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_S8, .manufacturer_names_count=sizeof(kManus_S8)/sizeof(kManus_S8[0]),
    .model="S8", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_S8_Tuy_S8, .exposes_count=sizeof(kExp_min_S8_Tuy_S8)/sizeof(kExp_min_S8_Tuy_S8[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_S8_Tuy_S8, .from_zigbee_count=sizeof(kFz_min_S8_Tuy_S8)/sizeof(kFz_min_S8_Tuy_S8[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
