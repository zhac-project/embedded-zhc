// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya CS-201Z — auto-generated.
// Soil moisture sensor
// z2m-source: tuya.ts #CS-201Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_CS_201Z[] = { "ZG-303Z", "TS0601" };
constexpr const char* kManus_CS_201Z[] = { "_TZE200_npj9bug3", "_TZE200_wrmhp6b3" };
}  // namespace

namespace {
const FzConverter* const kFz_min_CS_201Z_Tuy_CS_201Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_CS_201Z_Tuy_CS_201Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_CS_201Z_Tuy_CS_201Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_CS_201Z{
    .zigbee_models=kModels_CS_201Z, .zigbee_models_count=sizeof(kModels_CS_201Z)/sizeof(kModels_CS_201Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_CS_201Z, .manufacturer_names_count=sizeof(kManus_CS_201Z)/sizeof(kManus_CS_201Z[0]),
    .model="CS-201Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_CS_201Z_Tuy_CS_201Z, .exposes_count=sizeof(kExp_min_CS_201Z_Tuy_CS_201Z)/sizeof(kExp_min_CS_201Z_Tuy_CS_201Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_CS_201Z_Tuy_CS_201Z, .from_zigbee_count=sizeof(kFz_min_CS_201Z_Tuy_CS_201Z)/sizeof(kFz_min_CS_201Z_Tuy_CS_201Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
