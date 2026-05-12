// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZYXH_switch_24 — auto-generated.
// 24 gang switch
// z2m-source: tuya.ts #ZYXH_switch_24.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZYXH_switch_24[] = { "TS0601" };
constexpr const char* kManus_ZYXH_switch_24[] = { "_TZE204_vmcgja59", "_TZE284_vmcgja59" };
constexpr ::zhc::EndpointLabel kEndpoints_ZYXH_switch_24[] = { {"l1", 1}, {"l2", 1}, {"l3", 1}, {"l4", 1}, {"l5", 1}, {"l6", 1}, {"l7", 1}, {"l8", 1}, {"l9", 1}, {"l10", 1}, {"l11", 1}, {"l12", 1}, {"l13", 1}, {"l14", 1}, {"l15", 1}, {"l16", 1}, {"l17", 1}, {"l18", 1}, {"l19", 1}, {"l20", 1}, {"l21", 1}, {"l22", 1}, {"l23", 1}, {"l24", 1} };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZYXH_switch_24_Tuy_ZYXH_switch_24[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZYXH_switch_24_Tuy_ZYXH_switch_24[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZYXH_switch_24_Tuy_ZYXH_switch_24[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZYXH_switch_24{
    .zigbee_models=kModels_ZYXH_switch_24, .zigbee_models_count=sizeof(kModels_ZYXH_switch_24)/sizeof(kModels_ZYXH_switch_24[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZYXH_switch_24, .manufacturer_names_count=sizeof(kManus_ZYXH_switch_24)/sizeof(kManus_ZYXH_switch_24[0]),
    .model="ZYXH_switch_24", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZYXH_switch_24_Tuy_ZYXH_switch_24, .exposes_count=sizeof(kExp_min_ZYXH_switch_24_Tuy_ZYXH_switch_24)/sizeof(kExp_min_ZYXH_switch_24_Tuy_ZYXH_switch_24[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZYXH_switch_24_Tuy_ZYXH_switch_24, .from_zigbee_count=sizeof(kFz_min_ZYXH_switch_24_Tuy_ZYXH_switch_24)/sizeof(kFz_min_ZYXH_switch_24_Tuy_ZYXH_switch_24[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_ZYXH_switch_24,
    .endpoint_map_count = sizeof(kEndpoints_ZYXH_switch_24)/sizeof(kEndpoints_ZYXH_switch_24[0]),
};

}  // namespace zhc::devices::tuya
