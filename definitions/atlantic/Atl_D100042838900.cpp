// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Atlantic Group 100042838900, Thermor Equateur 5 radiator (z2m v26.115.1 window).
// hvacThermostat (local temperature, heating setpoint 7..28, system mode off/heat),
// occupancy and seMetering energy in Wh. The model id also arrives with a trailing
// space.
// z2m-source: atlantic.ts #100042838900.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::atlantic {
namespace {
// currentSummDelivered is in Wh (z2m forces multiplier 1 / divisor 1000).
constexpr ::zhc::generic::ZclAttrRow kMeterRows[] = { { 0x0000, "energy", 1000 } };
constexpr ::zhc::generic::ZclAttrMap kMeterMap{ kMeterRows, 1 };
constexpr FzConverter kFzEnergyWh = ::zhc::generic::zcl_attr_fz("seMetering", &kMeterMap);
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzOccupancy,
    &kFzEnergyWh,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kOpts2[] = { "off", "heat" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts2, 2},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "100042838900", "100042838900 " };
constexpr BindingSpec kBind[] = {
    {1, 0x0201},
    {1, 0x0406},
    {1, 0x0702},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0201, 0x0000, 0x29, 0, 3600, 10, 0},
    {1, 0x0201, 0x0012, 0x29, 0, 3600, 10, 0},
    {1, 0x0201, 0x001C, 0x30, 10, 3600, 0, 0},
    {1, 0x0406, 0x0000, 0x18, 0, 3600, 0, 0},
    {1, 0x0702, 0x0000, 0x25, 10, 3600, 100, 0},
};
}  // namespace

extern const PreparedDefinition kDef_D100042838900{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="100042838900", .vendor="Atlantic",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::atlantic
