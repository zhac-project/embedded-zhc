// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZY-M100-24GV2 — auto-generated.
// 24G MmWave radar human presence motion sensor
// z2m-source: tuya.ts #ZY-M100-24GV2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZY_M100_24GV2[] = { "TS0601" };
constexpr const char* kManus_ZY_M100_24GV2[] = { "_TZE204_7gclukjs" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZY_M100_24GV2{
    .zigbee_models=kModels_ZY_M100_24GV2, .zigbee_models_count=sizeof(kModels_ZY_M100_24GV2)/sizeof(kModels_ZY_M100_24GV2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZY_M100_24GV2, .manufacturer_names_count=sizeof(kManus_ZY_M100_24GV2)/sizeof(kManus_ZY_M100_24GV2[0]),
    .model="ZY-M100-24GV2", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2, .exposes_count=sizeof(kExp_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2)/sizeof(kExp_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2, .from_zigbee_count=sizeof(kFz_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2)/sizeof(kFz_min_ZY_M100_24GV2_Tuy_ZY_M100_24GV2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
