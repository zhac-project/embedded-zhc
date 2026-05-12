// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TZ-ZT01_GA4 — auto-generated.
// Temperature & humidity Sensor with external probe
// z2m-source: tuya.ts #TZ-ZT01_GA4.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TZ_ZT01_GA4[] = { "TS0601" };

}  // namespace

namespace {
const FzConverter* const kFz_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TZ_ZT01_GA4{
    .zigbee_models=kModels_TZ_ZT01_GA4, .zigbee_models_count=sizeof(kModels_TZ_ZT01_GA4)/sizeof(kModels_TZ_ZT01_GA4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TZ-ZT01_GA4", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4, .exposes_count=sizeof(kExp_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4)/sizeof(kExp_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4, .from_zigbee_count=sizeof(kFz_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4)/sizeof(kFz_min_TZ_ZT01_GA4_Tuy_TZ_ZT01_GA4[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
