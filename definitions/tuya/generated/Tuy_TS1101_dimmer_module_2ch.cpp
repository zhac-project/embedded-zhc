// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS1101_dimmer_module_2ch — auto-generated.
// Zigbee dimmer module 2 channel
// z2m-source: tuya.ts #TS1101_dimmer_module_2ch.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS1101_dimmer_module_2ch[] = { "TS1101" };
constexpr const char* kManus_TS1101_dimmer_module_2ch[] = { "_TZ3000_7ysdnebc" };
constexpr ::zhc::EndpointLabel kEndpoints_TS1101_dimmer_module_2ch[] = { {"l1", 1}, {"l2", 2} };

}  // namespace

namespace {
const FzConverter* const kFz_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS1101_dimmer_module_2ch{
    .zigbee_models=kModels_TS1101_dimmer_module_2ch, .zigbee_models_count=sizeof(kModels_TS1101_dimmer_module_2ch)/sizeof(kModels_TS1101_dimmer_module_2ch[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS1101_dimmer_module_2ch, .manufacturer_names_count=sizeof(kManus_TS1101_dimmer_module_2ch)/sizeof(kManus_TS1101_dimmer_module_2ch[0]),
    .model="TS1101_dimmer_module_2ch", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch, .exposes_count=sizeof(kExp_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch)/sizeof(kExp_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch, .from_zigbee_count=sizeof(kFz_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch)/sizeof(kFz_min_TS1101_dimmer_module_2ch_Tuy_TS1101_dimmer_module_2ch[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_TS1101_dimmer_module_2ch,
    .endpoint_map_count = sizeof(kEndpoints_TS1101_dimmer_module_2ch)/sizeof(kEndpoints_TS1101_dimmer_module_2ch[0]),
};

}  // namespace zhc::devices::tuya
