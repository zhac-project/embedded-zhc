// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-105NTH — auto-generated.
// Temperature and humidity sensor with probe
// z2m-source: tuya.ts #ZG-105NTH.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_105NTH[] = { "Excellux" };
constexpr const char* kManus_ZG_105NTH[] = { "NTCHT01" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_105NTH_Tuy_ZG_105NTH[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_105NTH_Tuy_ZG_105NTH[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_105NTH_Tuy_ZG_105NTH[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_105NTH{
    .zigbee_models=kModels_ZG_105NTH, .zigbee_models_count=sizeof(kModels_ZG_105NTH)/sizeof(kModels_ZG_105NTH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZG_105NTH, .manufacturer_names_count=sizeof(kManus_ZG_105NTH)/sizeof(kManus_ZG_105NTH[0]),
    .model="ZG-105NTH", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_105NTH_Tuy_ZG_105NTH, .exposes_count=sizeof(kExp_min_ZG_105NTH_Tuy_ZG_105NTH)/sizeof(kExp_min_ZG_105NTH_Tuy_ZG_105NTH[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_105NTH_Tuy_ZG_105NTH, .from_zigbee_count=sizeof(kFz_min_ZG_105NTH_Tuy_ZG_105NTH)/sizeof(kFz_min_ZG_105NTH_Tuy_ZG_105NTH[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
