// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya X5H-GB-B — auto-generated.
// Wall-mount thermostat
// z2m-source: tuya.ts #X5H-GB-B.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_X5H_GB_B[] = { "TS0601" };
constexpr const char* kManus_X5H_GB_B[] = { "_TZE200_2ekuz3dz" };
}  // namespace

namespace {
const FzConverter* const kFz_min_X5H_GB_B_Tuy_X5H_GB_B[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_X5H_GB_B_Tuy_X5H_GB_B[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_X5H_GB_B_Tuy_X5H_GB_B[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_X5H_GB_B{
    .zigbee_models=kModels_X5H_GB_B, .zigbee_models_count=sizeof(kModels_X5H_GB_B)/sizeof(kModels_X5H_GB_B[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_X5H_GB_B, .manufacturer_names_count=sizeof(kManus_X5H_GB_B)/sizeof(kManus_X5H_GB_B[0]),
    .model="X5H-GB-B", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_X5H_GB_B_Tuy_X5H_GB_B, .exposes_count=sizeof(kExp_min_X5H_GB_B_Tuy_X5H_GB_B)/sizeof(kExp_min_X5H_GB_B_Tuy_X5H_GB_B[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_X5H_GB_B_Tuy_X5H_GB_B, .from_zigbee_count=sizeof(kFz_min_X5H_GB_B_Tuy_X5H_GB_B)/sizeof(kFz_min_X5H_GB_B_Tuy_X5H_GB_B[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
