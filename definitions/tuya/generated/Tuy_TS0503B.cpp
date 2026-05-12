// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0503B — auto-generated.
// Zigbee RGB light
// z2m-source: tuya.ts #TS0503B.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0503B[] = { "TS0503B" };

}  // namespace

namespace {
const FzConverter* const kFz_min_TS0503B_Tuy_TS0503B[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0503B_Tuy_TS0503B[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0503B_Tuy_TS0503B[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0503B{
    .zigbee_models=kModels_TS0503B, .zigbee_models_count=sizeof(kModels_TS0503B)/sizeof(kModels_TS0503B[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TS0503B", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0503B_Tuy_TS0503B, .exposes_count=sizeof(kExp_min_TS0503B_Tuy_TS0503B)/sizeof(kExp_min_TS0503B_Tuy_TS0503B[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0503B_Tuy_TS0503B, .from_zigbee_count=sizeof(kFz_min_TS0503B_Tuy_TS0503B)/sizeof(kFz_min_TS0503B_Tuy_TS0503B[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
