// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-104PLV — auto-generated.
// PIR motion sensor, vibration sensor, and light sensor
// z2m-source: tuya.ts #ZG-104PLV.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_104PLV[] = { "Excellux" };
constexpr const char* kManus_ZG_104PLV[] = { "PIRIV01" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_104PLV_Tuy_ZG_104PLV[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_104PLV_Tuy_ZG_104PLV[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_104PLV_Tuy_ZG_104PLV[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_104PLV{
    .zigbee_models=kModels_ZG_104PLV, .zigbee_models_count=sizeof(kModels_ZG_104PLV)/sizeof(kModels_ZG_104PLV[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZG_104PLV, .manufacturer_names_count=sizeof(kManus_ZG_104PLV)/sizeof(kManus_ZG_104PLV[0]),
    .model="ZG-104PLV", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_104PLV_Tuy_ZG_104PLV, .exposes_count=sizeof(kExp_min_ZG_104PLV_Tuy_ZG_104PLV)/sizeof(kExp_min_ZG_104PLV_Tuy_ZG_104PLV[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_104PLV_Tuy_ZG_104PLV, .from_zigbee_count=sizeof(kFz_min_ZG_104PLV_Tuy_ZG_104PLV)/sizeof(kFz_min_ZG_104PLV_Tuy_ZG_104PLV[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
