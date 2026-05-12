// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya PN6 — auto-generated.
// 6-way controller
// z2m-source: tuya.ts #PN6.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_PN6[] = { "TS0601" };
constexpr const char* kManus_PN6[] = { "_TZE204_ncti2pro", "_TZE204_l8xiyymq", "_TZE284_l8xiyymq", "_TZE284_zeldawjv" };
constexpr ::zhc::EndpointLabel kEndpoints_PN6[] = { {"l1", 1}, {"l2", 1}, {"l3", 1}, {"l4", 1}, {"l5", 1}, {"l6", 1}, {"state", 1}, {"l1_l2", 1}, {"l3_l4", 1}, {"l5_l6", 1} };

}  // namespace

namespace {
const FzConverter* const kFz_min_PN6_Tuy_PN6[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_PN6_Tuy_PN6[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PN6_Tuy_PN6[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_PN6{
    .zigbee_models=kModels_PN6, .zigbee_models_count=sizeof(kModels_PN6)/sizeof(kModels_PN6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_PN6, .manufacturer_names_count=sizeof(kManus_PN6)/sizeof(kManus_PN6[0]),
    .model="PN6", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_PN6_Tuy_PN6, .exposes_count=sizeof(kExp_min_PN6_Tuy_PN6)/sizeof(kExp_min_PN6_Tuy_PN6[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PN6_Tuy_PN6, .from_zigbee_count=sizeof(kFz_min_PN6_Tuy_PN6)/sizeof(kFz_min_PN6_Tuy_PN6[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_PN6,
    .endpoint_map_count = sizeof(kEndpoints_PN6)/sizeof(kEndpoints_PN6[0]),
};

}  // namespace zhc::devices::tuya
