// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZS-300Z — auto-generated.
// Soil moisture sensor
// z2m-source: tuya.ts #ZS-300Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZS_300Z[] = { "ZS-304Z", "TS0601" };
constexpr const char* kManus_ZS_300Z[] = { "_TZE284_k7p2q5d9", "_TZE284_65gzcss7", "_TZE284_0ints6wl", "_TZE284_yzr43ayq" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZS_300Z_Tuy_ZS_300Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZS_300Z_Tuy_ZS_300Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZS_300Z_Tuy_ZS_300Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZS_300Z{
    .zigbee_models=kModels_ZS_300Z, .zigbee_models_count=sizeof(kModels_ZS_300Z)/sizeof(kModels_ZS_300Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZS_300Z, .manufacturer_names_count=sizeof(kManus_ZS_300Z)/sizeof(kManus_ZS_300Z[0]),
    .model="ZS-300Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZS_300Z_Tuy_ZS_300Z, .exposes_count=sizeof(kExp_min_ZS_300Z_Tuy_ZS_300Z)/sizeof(kExp_min_ZS_300Z_Tuy_ZS_300Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZS_300Z_Tuy_ZS_300Z, .from_zigbee_count=sizeof(kFz_min_ZS_300Z_Tuy_ZS_300Z)/sizeof(kFz_min_ZS_300Z_Tuy_ZS_300Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
