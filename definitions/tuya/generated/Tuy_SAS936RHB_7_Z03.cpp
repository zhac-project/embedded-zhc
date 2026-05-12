// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya SAS936RHB-7-Z03 — auto-generated.
// Wireless temperature sensor
// z2m-source: tuya.ts #SAS936RHB-7-Z03.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_SAS936RHB_7_Z03[] = { "TS0601" };
constexpr const char* kManus_SAS936RHB_7_Z03[] = { "_TZE284_madl8ejv" };
}  // namespace

namespace {
const FzConverter* const kFz_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_SAS936RHB_7_Z03{
    .zigbee_models=kModels_SAS936RHB_7_Z03, .zigbee_models_count=sizeof(kModels_SAS936RHB_7_Z03)/sizeof(kModels_SAS936RHB_7_Z03[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SAS936RHB_7_Z03, .manufacturer_names_count=sizeof(kManus_SAS936RHB_7_Z03)/sizeof(kManus_SAS936RHB_7_Z03[0]),
    .model="SAS936RHB-7-Z03", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03, .exposes_count=sizeof(kExp_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03)/sizeof(kExp_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03, .from_zigbee_count=sizeof(kFz_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03)/sizeof(kFz_min_SAS936RHB_7_Z03_Tuy_SAS936RHB_7_Z03[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
