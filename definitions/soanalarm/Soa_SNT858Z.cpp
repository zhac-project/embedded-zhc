// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Soanalarm SNT858Z — TS0601 Tuya-MCU soil moisture sensor.
//
// Bug fixed: the auto-port mis-classified this Tuya-DP sensor as a phantom
// on/off + battery device. It wired kFzOnOff + kFzBattery, exposed
// state/battery/voltage and bound genOnOff(0x0006) + genPowerCfg(0x0001).
// SNT858Z is a Tuya-MCU device (tuya.modernExtend.tuyaBase({dp:true})) — it
// speaks ONLY the 0xEF00 datapoint stream, so it decoded NOTHING real
// (no soil_moisture, temperature, temperature_unit, or battery) and carried a
// dead on/off bundle. The def now wires the z2m DP map:
//   dp3  soil_moisture     Numeric raw (/1)  → soil_moisture
//   dp5  temperature       Numeric raw (/1)  → temperature  (z2m valueConverter.raw,
//                                              NOT the /10 dpTemperature default)
//   dp9  temperature_unit  Enum {0:celsius,1:fahrenheit} → temperature_unit
//   dp15 battery           Numeric raw (/1)  → battery
//
// temperature_unit is STATE_SET in z2m, so the def uses factory::TuyaRw to
// also surface the DP write path. On-device write is host-tested only
// (no hardware).
//
// z2m-source: soanalarm.ts #SNT858Z (tuya.modernExtend.tuyaBase + tuyaDatapoints).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::soanalarm {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kTempUnit[] = {
    { 0, "celsius" }, { 1, "fahrenheit" },
};

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(3, "soil_moisture", 1),
        ::zhc::tuya::dp::numeric(5, "temperature", 1),  // z2m valueConverter.raw → /1
        ::zhc::tuya::dp::enum_lookup(9, "temperature_unit", kTempUnit, 2),
        ::zhc::tuya::dp::numeric(15, "battery", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, 4 };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kModels_SNT858Z[] = { "SNT858Z" };

constexpr const char* kTempUnitOpts[] = { "celsius", "fahrenheit" };

constexpr Expose kExp_SNT858Z[] = {
    { "temperature",      ExposeType::Numeric, ::zhc::Access::State,    "\xC2\xB0""C", nullptr, nullptr,       0 },
    { "soil_moisture",    ExposeType::Numeric, ::zhc::Access::State,    "%",           nullptr, nullptr,       0 },
    { "temperature_unit", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr,       nullptr, kTempUnitOpts, 2 },
    { "battery",          ExposeType::Numeric, ::zhc::Access::State,    "%",           nullptr, nullptr,       0 },
};

}  // namespace

extern const PreparedDefinition kDef_SNT858Z{
    .zigbee_models=kModels_SNT858Z, .zigbee_models_count=sizeof(kModels_SNT858Z)/sizeof(kModels_SNT858Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNT858Z", .vendor="Soanalarm",
    .meta=nullptr, .exposes=kExp_SNT858Z, .exposes_count=sizeof(kExp_SNT858Z)/sizeof(kExp_SNT858Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
};

}  // namespace zhc::devices::soanalarm
