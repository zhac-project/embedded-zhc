// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya Senoro.Win v2 — auto-generated.
// Window sensor with 3-state opening (DP101), optional alarm, battery
// z2m-source: tuya.ts #Senoro.Win v2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_Senoro_Win_v2[] = { "TS0601" };
constexpr const char* kManus_Senoro_Win_v2[] = { "_TZE284_6teua268" };
}  // namespace

namespace {
const FzConverter* const kFz_min_Senoro_Win_v2_Tuy_Senoro_Win_v2[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_Senoro_Win_v2_Tuy_Senoro_Win_v2[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_Senoro_Win_v2_Tuy_Senoro_Win_v2[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_Senoro_Win_v2{
    .zigbee_models=kModels_Senoro_Win_v2, .zigbee_models_count=sizeof(kModels_Senoro_Win_v2)/sizeof(kModels_Senoro_Win_v2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_Senoro_Win_v2, .manufacturer_names_count=sizeof(kManus_Senoro_Win_v2)/sizeof(kManus_Senoro_Win_v2[0]),
    .model="Senoro.Win v2", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_Senoro_Win_v2_Tuy_Senoro_Win_v2, .exposes_count=sizeof(kExp_min_Senoro_Win_v2_Tuy_Senoro_Win_v2)/sizeof(kExp_min_Senoro_Win_v2_Tuy_Senoro_Win_v2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_Senoro_Win_v2_Tuy_Senoro_Win_v2, .from_zigbee_count=sizeof(kFz_min_Senoro_Win_v2_Tuy_Senoro_Win_v2)/sizeof(kFz_min_Senoro_Win_v2_Tuy_Senoro_Win_v2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
