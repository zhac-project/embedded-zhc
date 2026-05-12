// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0052 — auto-generated.
// Zigbee dimmer module 1 channel
// z2m-source: tuya.ts #TS0052.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0052[] = { "TS0052" };

}  // namespace

namespace {
const FzConverter* const kFz_min_TS0052_Tuy_TS0052[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0052_Tuy_TS0052[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0052_Tuy_TS0052[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0052{
    .zigbee_models=kModels_TS0052, .zigbee_models_count=sizeof(kModels_TS0052)/sizeof(kModels_TS0052[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TS0052", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0052_Tuy_TS0052, .exposes_count=sizeof(kExp_min_TS0052_Tuy_TS0052)/sizeof(kExp_min_TS0052_Tuy_TS0052[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0052_Tuy_TS0052, .from_zigbee_count=sizeof(kFz_min_TS0052_Tuy_TS0052)/sizeof(kFz_min_TS0052_Tuy_TS0052[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
