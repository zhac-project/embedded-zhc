// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-227ZL — auto-generated.
// Temperature & humidity LCD sensor
// z2m-source: tuya.ts #ZG-227ZL.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_227ZL[] = { "ZG-227Z", "ZG-227ZL", "TS0601" };
constexpr const char* kManus_ZG_227ZL[] = { "_TZE200_qoy0ekbd", "_TZE200_znbl8dj5", "_TZE200_a8sdabtg", "_TZE200_dikkika5", "_TZE200_vs0skpuc", "_TZE200_3xfjp0ag", "_TZE200_ehhrv2e3", "_TZE200_lhqtjwax" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_227ZL_Tuy_ZG_227ZL[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_227ZL_Tuy_ZG_227ZL[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_227ZL_Tuy_ZG_227ZL[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_227ZL{
    .zigbee_models=kModels_ZG_227ZL, .zigbee_models_count=sizeof(kModels_ZG_227ZL)/sizeof(kModels_ZG_227ZL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZG_227ZL, .manufacturer_names_count=sizeof(kManus_ZG_227ZL)/sizeof(kManus_ZG_227ZL[0]),
    .model="ZG-227ZL", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_227ZL_Tuy_ZG_227ZL, .exposes_count=sizeof(kExp_min_ZG_227ZL_Tuy_ZG_227ZL)/sizeof(kExp_min_ZG_227ZL_Tuy_ZG_227ZL[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_227ZL_Tuy_ZG_227ZL, .from_zigbee_count=sizeof(kFz_min_ZG_227ZL_Tuy_ZG_227ZL)/sizeof(kFz_min_ZG_227ZL_Tuy_ZG_227ZL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
