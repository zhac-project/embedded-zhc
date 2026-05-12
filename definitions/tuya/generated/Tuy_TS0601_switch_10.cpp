// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_switch_10 — auto-generated.
// 10 gang switch
// z2m-source: tuya.ts #TS0601_switch_10.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_switch_10[] = { "TS0601" };
constexpr const char* kManus_TS0601_switch_10[] = { "_TZE200_7sjncirf", "TZE204_7sjncirf" };
constexpr ::zhc::EndpointLabel kEndpoints_TS0601_switch_10[] = { {"l1", 1}, {"l2", 1}, {"l3", 1}, {"l4", 1}, {"l5", 1}, {"l6", 1}, {"l7", 1}, {"l8", 1}, {"l9", 1}, {"l10", 1}, {"l11", 1}, {"l12", 1} };

}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_switch_10_Tuy_TS0601_switch_10[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_switch_10_Tuy_TS0601_switch_10[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_switch_10_Tuy_TS0601_switch_10[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_switch_10{
    .zigbee_models=kModels_TS0601_switch_10, .zigbee_models_count=sizeof(kModels_TS0601_switch_10)/sizeof(kModels_TS0601_switch_10[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_switch_10, .manufacturer_names_count=sizeof(kManus_TS0601_switch_10)/sizeof(kManus_TS0601_switch_10[0]),
    .model="TS0601_switch_10", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_switch_10_Tuy_TS0601_switch_10, .exposes_count=sizeof(kExp_min_TS0601_switch_10_Tuy_TS0601_switch_10)/sizeof(kExp_min_TS0601_switch_10_Tuy_TS0601_switch_10[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_switch_10_Tuy_TS0601_switch_10, .from_zigbee_count=sizeof(kFz_min_TS0601_switch_10_Tuy_TS0601_switch_10)/sizeof(kFz_min_TS0601_switch_10_Tuy_TS0601_switch_10[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_TS0601_switch_10,
    .endpoint_map_count = sizeof(kEndpoints_TS0601_switch_10)/sizeof(kEndpoints_TS0601_switch_10[0]),
};

}  // namespace zhc::devices::tuya
