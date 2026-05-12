// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TE-1Z — auto-generated.
// Floor heating thermostat
// z2m-source: tuya.ts #TE-1Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TE_1Z[] = { "TS0601" };
constexpr const char* kManus_TE_1Z[] = { "_TZE284_khah2lkr" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TE_1Z_Tuy_TE_1Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TE_1Z_Tuy_TE_1Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TE_1Z_Tuy_TE_1Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TE_1Z{
    .zigbee_models=kModels_TE_1Z, .zigbee_models_count=sizeof(kModels_TE_1Z)/sizeof(kModels_TE_1Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TE_1Z, .manufacturer_names_count=sizeof(kManus_TE_1Z)/sizeof(kManus_TE_1Z[0]),
    .model="TE-1Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TE_1Z_Tuy_TE_1Z, .exposes_count=sizeof(kExp_min_TE_1Z_Tuy_TE_1Z)/sizeof(kExp_min_TE_1Z_Tuy_TE_1Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TE_1Z_Tuy_TE_1Z, .from_zigbee_count=sizeof(kFz_min_TE_1Z_Tuy_TE_1Z)/sizeof(kFz_min_TE_1Z_Tuy_TE_1Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
