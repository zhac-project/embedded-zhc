// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-205Z — auto-generated.
// 5.8 GHz human presence sensor
// z2m-source: tuya.ts #ZG-205Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_205Z[] = { "TS0601" };
constexpr const char* kManus_ZG_205Z[] = { "_TZE204_dapwryy7" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_205Z_Tuy_ZG_205Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_205Z_Tuy_ZG_205Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_205Z_Tuy_ZG_205Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_205Z{
    .zigbee_models=kModels_ZG_205Z, .zigbee_models_count=sizeof(kModels_ZG_205Z)/sizeof(kModels_ZG_205Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZG_205Z, .manufacturer_names_count=sizeof(kManus_ZG_205Z)/sizeof(kManus_ZG_205Z[0]),
    .model="ZG-205Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_205Z_Tuy_ZG_205Z, .exposes_count=sizeof(kExp_min_ZG_205Z_Tuy_ZG_205Z)/sizeof(kExp_min_ZG_205Z_Tuy_ZG_205Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_205Z_Tuy_ZG_205Z, .from_zigbee_count=sizeof(kFz_min_ZG_205Z_Tuy_ZG_205Z)/sizeof(kFz_min_ZG_205Z_Tuy_ZG_205Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
