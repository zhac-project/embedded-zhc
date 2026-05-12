// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-223Z — auto-generated.
// Rainwater detection sensor
// z2m-source: tuya.ts #ZG-223Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_223Z[] = { "ZG-223Z", "TS0601" };
constexpr const char* kManus_ZG_223Z[] = { "_TZE200_jsaqgakf", "_TZE200_u6x1zyv2", "_TZE200_2pddnnrk" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_223Z_Tuy_ZG_223Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_223Z_Tuy_ZG_223Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_223Z_Tuy_ZG_223Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_223Z{
    .zigbee_models=kModels_ZG_223Z, .zigbee_models_count=sizeof(kModels_ZG_223Z)/sizeof(kModels_ZG_223Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZG_223Z, .manufacturer_names_count=sizeof(kManus_ZG_223Z)/sizeof(kManus_ZG_223Z[0]),
    .model="ZG-223Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_223Z_Tuy_ZG_223Z, .exposes_count=sizeof(kExp_min_ZG_223Z_Tuy_ZG_223Z)/sizeof(kExp_min_ZG_223Z_Tuy_ZG_223Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_223Z_Tuy_ZG_223Z, .from_zigbee_count=sizeof(kFz_min_ZG_223Z_Tuy_ZG_223Z)/sizeof(kFz_min_ZG_223Z_Tuy_ZG_223Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
