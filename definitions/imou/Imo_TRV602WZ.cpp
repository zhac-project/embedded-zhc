// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Topband TRV602WZ smart thermostat (z2m v26.125.0 window, imou.ts).
// m.battery + m.identify + m.onOff + m.thermostat (local temperature,
// heating setpoint 7-30 °C, system_mode off/heat, running_state idle/heat).
// Upstream's deviceEndpoints {1, 2} skips every key the def publishes, so all
// keys stay unsuffixed and no endpoint map is needed here.
// z2m-source: imou.ts #TRV602WZ.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::imou {
namespace {

constexpr ::zhc::generic::ZclWriteLookup kRunningStates[] = { {"idle", 0}, {"heat", 1} };
constexpr ::zhc::generic::ZclAttrRow kRowsThermostat[] = {
    {0x0029, "running_state", 1, kRunningStates, 2},
};
constexpr ::zhc::generic::ZclAttrMap kMapThermostat{ kRowsThermostat, sizeof(kRowsThermostat)/sizeof(kRowsThermostat[0]) };
constexpr FzConverter kFzRunningState = ::zhc::generic::zcl_attr_fz("hvacThermostat", &kMapThermostat);

const FzConverter* const kFz_TRV602WZ[] = {
    &::zhc::generic::kFzThermostat,
    &kFzRunningState,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_TRV602WZ[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_TRV602WZ[] = { "TRV602WZ" };

constexpr const char* kSystemModes[] = { "off", "heat" };
constexpr const char* kRunningStateOpts[] = { "idle", "heat" };
constexpr Expose kExp_TRV602WZ[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
     ExposeCategory::State, 7, 30, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModes, 2},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kRunningStateOpts, 2},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_TRV602WZ[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0201},
};

}  // namespace

extern const PreparedDefinition kDef_TRV602WZ{
    .zigbee_models=kModels_TRV602WZ, .zigbee_models_count=sizeof(kModels_TRV602WZ)/sizeof(kModels_TRV602WZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TRV602WZ", .vendor="Topband",
    .meta=nullptr, .exposes=kExp_TRV602WZ, .exposes_count=sizeof(kExp_TRV602WZ)/sizeof(kExp_TRV602WZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TRV602WZ, .from_zigbee_count=sizeof(kFz_TRV602WZ)/sizeof(kFz_TRV602WZ[0]),
    .to_zigbee=kTz_TRV602WZ, .to_zigbee_count=sizeof(kTz_TRV602WZ)/sizeof(kTz_TRV602WZ[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TRV602WZ, .bindings_count=sizeof(kBindings_TRV602WZ)/sizeof(kBindings_TRV602WZ[0]),
};

}  // namespace zhc::devices::imou
