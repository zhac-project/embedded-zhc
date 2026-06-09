// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Yandex YNDX-00518 — graduated from generated/ (vendor-parity sweep).
// Thermostatic radiator valve. z2m wires m.thermostat() over hvacThermostat
// (0x0201): LocalTemperature (0x0000), OccupiedHeatingSetpoint (0x0012) and
// localTemperatureCalibration, plus m.onOff() and a child_lock / display_flip /
// calibrated trio. The generated def decoded ONLY genOnOff + battery and bound
// neither hvacThermostat nor any decoder for it — so the entire thermostat
// surface (local_temperature, setpoint, mode) was missing from the shadow.
//
// Wire the generic thermostat read decoder (kFzThermostat → local_temperature,
// current_heating_setpoint, system_mode) and the matching write encoder
// (kTzThermostat: setpoint °C with x100 scaling + system_mode), plus the
// already-built Yandex display_flip write. Exposes kept FLAT (project rule —
// no ExposeType::Climate). local_temperature_calibration (0x0010), child_lock
// (hvacUserInterfaceCfg) and the manuSpecific `calibrated` flag still need
// generic helpers that do not exist yet → deferred (INFRA).
// z2m-source: yandex.ts #YNDX-00518 (m.thermostat() + m.onOff()).
#include "definitions/_generic/_shared.hpp"
#include "definitions/yandex/_shared.hpp"  // kTzYxDisplayFlip

namespace zhc::devices::yandex {
namespace {
const FzConverter* const kFz_YNDX_00518[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_YNDX_00518[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzThermostat,
    &::zhc::yandex::kTzYxDisplayFlip,
};
constexpr const char* kModels_YNDX_00518[] = { "YNDX-00518" };

}  // namespace


// --- exposes / bindings (graduated; flat thermostat surface) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"display_flip", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0201},  // hvacThermostat (local_temperature / setpoint / system_mode)
    {1, 0x0001},  // genPowerCfg
};
// --- end ---

extern const PreparedDefinition kDef_YNDX_00518{
    .zigbee_models=kModels_YNDX_00518, .zigbee_models_count=sizeof(kModels_YNDX_00518)/sizeof(kModels_YNDX_00518[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX-00518", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00518, .from_zigbee_count=sizeof(kFz_YNDX_00518)/sizeof(kFz_YNDX_00518[0]),
    .to_zigbee=kTz_YNDX_00518, .to_zigbee_count=sizeof(kTz_YNDX_00518)/sizeof(kTz_YNDX_00518[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yandex
