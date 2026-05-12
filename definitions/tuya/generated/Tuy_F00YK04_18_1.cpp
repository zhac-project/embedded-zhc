// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya F00YK04-18-1 — auto-generated.
// 18 scenes remote
// z2m-source: tuya.ts #F00YK04-18-1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_F00YK04_18_1[] = { "TS0601" };
constexpr const char* kManus_F00YK04_18_1[] = { "_TZE200_dhke3p9w", "_TZE284_dhke3p9w" };
}  // namespace

namespace {
const FzConverter* const kFz_min_F00YK04_18_1_Tuy_F00YK04_18_1[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_F00YK04_18_1_Tuy_F00YK04_18_1[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_F00YK04_18_1_Tuy_F00YK04_18_1[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_F00YK04_18_1{
    .zigbee_models=kModels_F00YK04_18_1, .zigbee_models_count=sizeof(kModels_F00YK04_18_1)/sizeof(kModels_F00YK04_18_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_F00YK04_18_1, .manufacturer_names_count=sizeof(kManus_F00YK04_18_1)/sizeof(kManus_F00YK04_18_1[0]),
    .model="F00YK04-18-1", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_F00YK04_18_1_Tuy_F00YK04_18_1, .exposes_count=sizeof(kExp_min_F00YK04_18_1_Tuy_F00YK04_18_1)/sizeof(kExp_min_F00YK04_18_1_Tuy_F00YK04_18_1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_F00YK04_18_1_Tuy_F00YK04_18_1, .from_zigbee_count=sizeof(kFz_min_F00YK04_18_1_Tuy_F00YK04_18_1)/sizeof(kFz_min_F00YK04_18_1_Tuy_F00YK04_18_1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
