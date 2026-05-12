// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZS-SF00 — auto-generated.
// Soil fertility sensor
// z2m-source: tuya.ts #ZS-SF00.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZS_SF00[] = { "Arteco" };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZS_SF00_Tuy_ZS_SF00[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZS_SF00_Tuy_ZS_SF00[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZS_SF00_Tuy_ZS_SF00[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZS_SF00{
    .zigbee_models=kModels_ZS_SF00, .zigbee_models_count=sizeof(kModels_ZS_SF00)/sizeof(kModels_ZS_SF00[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZS-SF00", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZS_SF00_Tuy_ZS_SF00, .exposes_count=sizeof(kExp_min_ZS_SF00_Tuy_ZS_SF00)/sizeof(kExp_min_ZS_SF00_Tuy_ZS_SF00[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZS_SF00_Tuy_ZS_SF00, .from_zigbee_count=sizeof(kFz_min_ZS_SF00_Tuy_ZS_SF00)/sizeof(kFz_min_ZS_SF00_Tuy_ZS_SF00[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
