// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZS-300TF — auto-generated.
// Soil fertility sensor
// z2m-source: tuya.ts #ZS-300TF.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZS_300TF[] = { "TS0601" };
constexpr const char* kManus_ZS_300TF[] = { "_TZE284_hdml1aav" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZS_300TF_Tuy_ZS_300TF[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZS_300TF_Tuy_ZS_300TF[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZS_300TF_Tuy_ZS_300TF[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZS_300TF{
    .zigbee_models=kModels_ZS_300TF, .zigbee_models_count=sizeof(kModels_ZS_300TF)/sizeof(kModels_ZS_300TF[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZS_300TF, .manufacturer_names_count=sizeof(kManus_ZS_300TF)/sizeof(kManus_ZS_300TF[0]),
    .model="ZS-300TF", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZS_300TF_Tuy_ZS_300TF, .exposes_count=sizeof(kExp_min_ZS_300TF_Tuy_ZS_300TF)/sizeof(kExp_min_ZS_300TF_Tuy_ZS_300TF[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZS_300TF_Tuy_ZS_300TF, .from_zigbee_count=sizeof(kFz_min_ZS_300TF_Tuy_ZS_300TF)/sizeof(kFz_min_ZS_300TF_Tuy_ZS_300TF[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
