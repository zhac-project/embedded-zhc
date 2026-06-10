// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Evanell EZ200 — Tuya-MCU thermostatic radiator valve.
//
// The auto-port wired this device to the generic genThermostat converter
// (cluster 0x0201). That is WRONG: EZ200 is a Tuya-MCU device that speaks
// the 0xEF00 datapoint stream (manuSpecificTuya), NOT genThermostat. Under
// the generic converter the device decoded nothing and the genThermostat
// 0x0201 binding was a phantom. Re-wire to the Tuya DP map.
//
// z2m-source: evanell.ts #EZ200 + legacy.ts fz.evanell_thermostat /
//             tz.evanell_thermostat_* (dataPoints.evanell* = 2/4/5/6/8).
//
// DP map (legacy.ts):
//   dp2 evanellMode            enum  → system_mode  {0:auto, 2:heat, 3:off}
//   dp4 evanellHeatingSetpoint Numeric /10 → current_heating_setpoint
//   dp5 evanellLocalTemp       Numeric /10 → local_temperature
//   dp6 evanellBattery         Numeric raw → battery
//   dp8 evanellChildLock       Bool        → child_lock
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::evanell {
namespace {

// z2m legacy.ts evanell_thermostat mode decode: wire 0→"auto", 2→"heat",
// 3→"off" (the inline source comments next to each case are mislabelled in
// z2m; the EMITTED system_mode string is what matters and is mirrored here).
static constexpr ::zhc::tuya::TuyaEnumEntry kEvanellModes[] = {
    {0, "auto"},
    {2, "heat"},
    {3, "off"},
};

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry entries[] = {
        ::zhc::tuya::dp::enum_lookup(2, "system_mode", kEvanellModes, 3),
        ::zhc::tuya::dp::numeric(4, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(5, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(6, "battery"),
        ::zhc::tuya::dp::binary(8, "child_lock"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{entries, 5};
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kModels_EZ200[] = { "TS0601" };
constexpr const char* kManus_EZ200[]  = { "_TZE200_dmfguuli", "_TZE200_rxypyjkw" };

// FLAT exposes (project rule: no ExposeType::Climate). z2m exposes:
// child_lock, battery, climate(current_heating_setpoint 5..30 step 0.5,
// local_temperature, system_mode {off,heat,auto}).
constexpr const char* kSystemModeValues[] = { "off", "heat", "auto" };
constexpr Expose kExposes_EZ200[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr,
     nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr,
     kSystemModeValues, 3},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

}  // namespace

extern const PreparedDefinition kDef_EZ200{
    .zigbee_models=kModels_EZ200, .zigbee_models_count=sizeof(kModels_EZ200)/sizeof(kModels_EZ200[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_EZ200, .manufacturer_names_count=sizeof(kManus_EZ200)/sizeof(kManus_EZ200[0]),
    .model="EZ200", .vendor="Evanell",
    .meta=nullptr, .exposes=kExposes_EZ200, .exposes_count=sizeof(kExposes_EZ200)/sizeof(kExposes_EZ200[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::evanell
