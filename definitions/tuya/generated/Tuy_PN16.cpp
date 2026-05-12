// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya PN16 — auto-generated.
// 16-way controller
// z2m-source: tuya.ts #PN16.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_PN16[] = { "TS0601" };
constexpr const char* kManus_PN16[] = { "_TZE204_zqq3cipq", "_TZE284_zqq3cipq" };
constexpr ::zhc::EndpointLabel kEndpoints_PN16[] = { {"all", 1}, {"l1", 1}, {"l2", 1}, {"l3", 1}, {"l4", 1}, {"l5", 1}, {"l6", 1}, {"l7", 1}, {"l8", 1}, {"l9", 1}, {"l10", 1}, {"l11", 1}, {"l12", 1}, {"l13", 1}, {"l14", 1}, {"l15", 1}, {"l16", 1}, {"l1_l8", 1}, {"l9_l16", 1}, {"l11_l12", 1}, {"l13_l14", 1}, {"l15_l16", 1} };

}  // namespace

namespace {
const FzConverter* const kFz_min_PN16_Tuy_PN16[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_PN16_Tuy_PN16[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PN16_Tuy_PN16[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_PN16{
    .zigbee_models=kModels_PN16, .zigbee_models_count=sizeof(kModels_PN16)/sizeof(kModels_PN16[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_PN16, .manufacturer_names_count=sizeof(kManus_PN16)/sizeof(kManus_PN16[0]),
    .model="PN16", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_PN16_Tuy_PN16, .exposes_count=sizeof(kExp_min_PN16_Tuy_PN16)/sizeof(kExp_min_PN16_Tuy_PN16[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PN16_Tuy_PN16, .from_zigbee_count=sizeof(kFz_min_PN16_Tuy_PN16)/sizeof(kFz_min_PN16_Tuy_PN16[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_PN16,
    .endpoint_map_count = sizeof(kEndpoints_PN16)/sizeof(kEndpoints_PN16[0]),
};

}  // namespace zhc::devices::tuya
