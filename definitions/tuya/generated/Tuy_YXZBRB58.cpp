// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya YXZBRB58 — auto-generated.
// Smart human presence sensor
// z2m-source: tuya.ts #YXZBRB58.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_YXZBRB58[] = { "TS0601" };
constexpr const char* kManus_YXZBRB58[] = { "_TZE204_sooucan5", "_TZE204_oqtpvx51" };
}  // namespace

namespace {
const FzConverter* const kFz_min_YXZBRB58_Tuy_YXZBRB58[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_YXZBRB58_Tuy_YXZBRB58[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_YXZBRB58_Tuy_YXZBRB58[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_YXZBRB58{
    .zigbee_models=kModels_YXZBRB58, .zigbee_models_count=sizeof(kModels_YXZBRB58)/sizeof(kModels_YXZBRB58[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_YXZBRB58, .manufacturer_names_count=sizeof(kManus_YXZBRB58)/sizeof(kManus_YXZBRB58[0]),
    .model="YXZBRB58", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_YXZBRB58_Tuy_YXZBRB58, .exposes_count=sizeof(kExp_min_YXZBRB58_Tuy_YXZBRB58)/sizeof(kExp_min_YXZBRB58_Tuy_YXZBRB58[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_YXZBRB58_Tuy_YXZBRB58, .from_zigbee_count=sizeof(kFz_min_YXZBRB58_Tuy_YXZBRB58)/sizeof(kFz_min_YXZBRB58_Tuy_YXZBRB58[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
