// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZR360CDB — auto-generated.
// Multifunctional CO2 detector
// z2m-source: tuya.ts #ZR360CDB.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZR360CDB[] = { "TS0601" };
constexpr const char* kManus_ZR360CDB[] = { "_TZE200_pl31aqf5", "_TZE200_xpvamyfz", "_TZE284_xpvamyfz" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZR360CDB_Tuy_ZR360CDB[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZR360CDB_Tuy_ZR360CDB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZR360CDB_Tuy_ZR360CDB[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZR360CDB{
    .zigbee_models=kModels_ZR360CDB, .zigbee_models_count=sizeof(kModels_ZR360CDB)/sizeof(kModels_ZR360CDB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZR360CDB, .manufacturer_names_count=sizeof(kManus_ZR360CDB)/sizeof(kManus_ZR360CDB[0]),
    .model="ZR360CDB", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZR360CDB_Tuy_ZR360CDB, .exposes_count=sizeof(kExp_min_ZR360CDB_Tuy_ZR360CDB)/sizeof(kExp_min_ZR360CDB_Tuy_ZR360CDB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZR360CDB_Tuy_ZR360CDB, .from_zigbee_count=sizeof(kFz_min_ZR360CDB_Tuy_ZR360CDB)/sizeof(kFz_min_ZR360CDB_Tuy_ZR360CDB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
