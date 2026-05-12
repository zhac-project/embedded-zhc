// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-204ZQ — auto-generated.
// PIR temperature&humidity sensor
// z2m-source: tuya.ts #ZG-204ZQ.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_204ZQ[] = { "ZG-204ZQ", "TS0601" };
constexpr const char* kManus_ZG_204ZQ[] = { "_TZE200_p9zbdqgs" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_204ZQ_Tuy_ZG_204ZQ[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_204ZQ_Tuy_ZG_204ZQ[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_204ZQ_Tuy_ZG_204ZQ[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_204ZQ{
    .zigbee_models=kModels_ZG_204ZQ, .zigbee_models_count=sizeof(kModels_ZG_204ZQ)/sizeof(kModels_ZG_204ZQ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZG_204ZQ, .manufacturer_names_count=sizeof(kManus_ZG_204ZQ)/sizeof(kManus_ZG_204ZQ[0]),
    .model="ZG-204ZQ", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_204ZQ_Tuy_ZG_204ZQ, .exposes_count=sizeof(kExp_min_ZG_204ZQ_Tuy_ZG_204ZQ)/sizeof(kExp_min_ZG_204ZQ_Tuy_ZG_204ZQ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_204ZQ_Tuy_ZG_204ZQ, .from_zigbee_count=sizeof(kFz_min_ZG_204ZQ_Tuy_ZG_204ZQ)/sizeof(kFz_min_ZG_204ZQ_Tuy_ZG_204ZQ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
