// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Mill Mill-gen-4 — hand-fixed wrong-class port.
// WiFi heating panel gen4.
//
// z2m-source: mill.ts #Mill-gen-4 — m.identify() + m.thermostat({
//   setpoints.occupiedHeatingSetpoint {5..35 step 0.5}, systemMode ["off","heat"] }).
//
// The auto-port mis-modelled this mains electric heating panel as a bare
// genOnOff switch + battery (phantom state/battery/voltage, bind 0x0001/0x0006).
// It is a hvacThermostat (0x0201): m.thermostat() with localTemperature not
// skipped exposes local_temperature (attr 0x0000) + a settable
// occupied/current_heating_setpoint (0x0012) + system_mode (0x001C). Wired the
// generic kFzThermostat read + kTzThermostat write and bound 0x0201. No battery
// cluster on this mains heater (z2m exposes none).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mill {
namespace {


constexpr const char* kModels_Mill_gen_4[] = { "Panel gen4", "Mill Wi-Fi Panel Heater Gen4" };

const FzConverter* const kFz_Mill_gen_4[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_Mill_gen_4[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr Expose kExp_Mill_gen_4[] = {
    { "local_temperature",        ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "current_heating_setpoint", ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "system_mode",              ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_Mill_gen_4[] = {
    { 1, 0x0201 },
};

}  // namespace

extern const PreparedDefinition kDef_Mill_gen_4{
    .zigbee_models=kModels_Mill_gen_4, .zigbee_models_count=sizeof(kModels_Mill_gen_4)/sizeof(kModels_Mill_gen_4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Mill-gen-4", .vendor="Mill",
    .meta=nullptr, .exposes=kExp_Mill_gen_4, .exposes_count=sizeof(kExp_Mill_gen_4)/sizeof(kExp_Mill_gen_4[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Mill_gen_4, .from_zigbee_count=sizeof(kFz_Mill_gen_4)/sizeof(kFz_Mill_gen_4[0]),
    .to_zigbee=kTz_Mill_gen_4, .to_zigbee_count=sizeof(kTz_Mill_gen_4)/sizeof(kTz_Mill_gen_4[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_Mill_gen_4,
    .bindings_count           = sizeof(kBind_Mill_gen_4)/sizeof(kBind_Mill_gen_4[0]),
};

}  // namespace zhc::devices::mill
