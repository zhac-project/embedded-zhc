// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0052_2 — auto-generated.
// Zigbee dimmer module 2 channel
// z2m-source: tuya.ts #TS0052_2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0052_2[] = { "TS0052" };
constexpr const char* kManus_TS0052_2[] = { "_TZ3000_zjtxnoft", "_TZ3000_kvwrdf47" };
constexpr ::zhc::EndpointLabel kEndpoints_TS0052_2[] = { {"l1", 1}, {"l2", 2} };

}  // namespace

namespace {
const FzConverter* const kFz_min_TS0052_2_Tuy_TS0052_2[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0052_2_Tuy_TS0052_2[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0052_2_Tuy_TS0052_2[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0052_2{
    .zigbee_models=kModels_TS0052_2, .zigbee_models_count=sizeof(kModels_TS0052_2)/sizeof(kModels_TS0052_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0052_2, .manufacturer_names_count=sizeof(kManus_TS0052_2)/sizeof(kManus_TS0052_2[0]),
    .model="TS0052_2", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0052_2_Tuy_TS0052_2, .exposes_count=sizeof(kExp_min_TS0052_2_Tuy_TS0052_2)/sizeof(kExp_min_TS0052_2_Tuy_TS0052_2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0052_2_Tuy_TS0052_2, .from_zigbee_count=sizeof(kFz_min_TS0052_2_Tuy_TS0052_2)/sizeof(kFz_min_TS0052_2_Tuy_TS0052_2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_TS0052_2,
    .endpoint_map_count = sizeof(kEndpoints_TS0052_2)/sizeof(kEndpoints_TS0052_2[0]),
};

}  // namespace zhc::devices::tuya
