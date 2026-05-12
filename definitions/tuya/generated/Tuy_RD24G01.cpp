// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya RD24G01 — auto-generated.
// 24GHz human presence sensor
// z2m-source: tuya.ts #RD24G01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_RD24G01[] = { "TS0601" };
constexpr const char* kManus_RD24G01[] = { "_TZE204_no6qtgtl" };
}  // namespace

namespace {
const FzConverter* const kFz_min_RD24G01_Tuy_RD24G01[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_RD24G01_Tuy_RD24G01[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_RD24G01_Tuy_RD24G01[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_RD24G01{
    .zigbee_models=kModels_RD24G01, .zigbee_models_count=sizeof(kModels_RD24G01)/sizeof(kModels_RD24G01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_RD24G01, .manufacturer_names_count=sizeof(kManus_RD24G01)/sizeof(kManus_RD24G01[0]),
    .model="RD24G01", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_RD24G01_Tuy_RD24G01, .exposes_count=sizeof(kExp_min_RD24G01_Tuy_RD24G01)/sizeof(kExp_min_RD24G01_Tuy_RD24G01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_RD24G01_Tuy_RD24G01, .from_zigbee_count=sizeof(kFz_min_RD24G01_Tuy_RD24G01)/sizeof(kFz_min_RD24G01_Tuy_RD24G01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
