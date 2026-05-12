// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya HHST001 — auto-generated.
// Fan coil thermostat
// z2m-source: tuya.ts #HHST001.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_HHST001[] = { "TS0601" };
constexpr const char* kManus_HHST001[] = { "_TZE204_q12rv9gj" };
}  // namespace

namespace {
const FzConverter* const kFz_min_HHST001_Tuy_HHST001[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_HHST001_Tuy_HHST001[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_HHST001_Tuy_HHST001[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_HHST001{
    .zigbee_models=kModels_HHST001, .zigbee_models_count=sizeof(kModels_HHST001)/sizeof(kModels_HHST001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_HHST001, .manufacturer_names_count=sizeof(kManus_HHST001)/sizeof(kManus_HHST001[0]),
    .model="HHST001", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_HHST001_Tuy_HHST001, .exposes_count=sizeof(kExp_min_HHST001_Tuy_HHST001)/sizeof(kExp_min_HHST001_Tuy_HHST001[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_HHST001_Tuy_HHST001, .from_zigbee_count=sizeof(kFz_min_HHST001_Tuy_HHST001)/sizeof(kFz_min_HHST001_Tuy_HHST001[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
