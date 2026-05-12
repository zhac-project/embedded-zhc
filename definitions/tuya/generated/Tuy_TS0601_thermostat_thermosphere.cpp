// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_thermostat_thermosphere — auto-generated.
// ThermoSphere thermostat
// z2m-source: tuya.ts #TS0601_thermostat_thermosphere.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_thermostat_thermosphere[] = { "TS0601" };
constexpr const char* kManus_TS0601_thermostat_thermosphere[] = { "_TZE200_ha0vwoew" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_thermostat_thermosphere{
    .zigbee_models=kModels_TS0601_thermostat_thermosphere, .zigbee_models_count=sizeof(kModels_TS0601_thermostat_thermosphere)/sizeof(kModels_TS0601_thermostat_thermosphere[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_thermostat_thermosphere, .manufacturer_names_count=sizeof(kManus_TS0601_thermostat_thermosphere)/sizeof(kManus_TS0601_thermostat_thermosphere[0]),
    .model="TS0601_thermostat_thermosphere", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos, .exposes_count=sizeof(kExp_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos)/sizeof(kExp_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos, .from_zigbee_count=sizeof(kFz_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos)/sizeof(kFz_min_TS0601_thermostat_thermosphere_Tuy_TS0601_thermostat_thermos[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
