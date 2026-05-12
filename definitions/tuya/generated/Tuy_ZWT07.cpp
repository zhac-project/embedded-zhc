// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZWT07 — auto-generated.
// Wall-mount thermostat
// z2m-source: tuya.ts #ZWT07.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZWT07[] = { "TS0601" };
constexpr const char* kManus_ZWT07[] = { "_TZE200_g9a3awaj" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZWT07_Tuy_ZWT07[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZWT07_Tuy_ZWT07[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZWT07_Tuy_ZWT07[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZWT07{
    .zigbee_models=kModels_ZWT07, .zigbee_models_count=sizeof(kModels_ZWT07)/sizeof(kModels_ZWT07[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZWT07, .manufacturer_names_count=sizeof(kManus_ZWT07)/sizeof(kManus_ZWT07[0]),
    .model="ZWT07", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZWT07_Tuy_ZWT07, .exposes_count=sizeof(kExp_min_ZWT07_Tuy_ZWT07)/sizeof(kExp_min_ZWT07_Tuy_ZWT07[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZWT07_Tuy_ZWT07, .from_zigbee_count=sizeof(kFz_min_ZWT07_Tuy_ZWT07)/sizeof(kFz_min_ZWT07_Tuy_ZWT07[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
