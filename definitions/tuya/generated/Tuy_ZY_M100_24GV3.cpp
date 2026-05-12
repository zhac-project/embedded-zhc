// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZY-M100-24GV3 — auto-generated.
// 24G MmWave radar human presence motion sensor (added distance switch)
// z2m-source: tuya.ts #ZY-M100-24GV3.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZY_M100_24GV3[] = { "TS0601" };
constexpr const char* kManus_ZY_M100_24GV3[] = { "_TZE204_ya4ft0w4", "_TZE200_ya4ft0w4", "_TZE204_gkfbdvyx", "_TZE200_gkfbdvyx" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZY_M100_24GV3{
    .zigbee_models=kModels_ZY_M100_24GV3, .zigbee_models_count=sizeof(kModels_ZY_M100_24GV3)/sizeof(kModels_ZY_M100_24GV3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZY_M100_24GV3, .manufacturer_names_count=sizeof(kManus_ZY_M100_24GV3)/sizeof(kManus_ZY_M100_24GV3[0]),
    .model="ZY-M100-24GV3", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3, .exposes_count=sizeof(kExp_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3)/sizeof(kExp_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3, .from_zigbee_count=sizeof(kFz_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3)/sizeof(kFz_min_ZY_M100_24GV3_Tuy_ZY_M100_24GV3[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
