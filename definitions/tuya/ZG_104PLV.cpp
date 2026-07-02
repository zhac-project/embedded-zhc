// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Excellux ZG-104PLV PIR + vibration + light sensor (Excellux/PIRIV01,
// z2m v26.77.0 parity). z2m-source: tuya.ts #ZG-104PLV. Graduated from a broken
// auto-extract stub that rendered this Tuya-DP (0xEF00) sensor as a genOnOff
// switch (phantom state/voltage + kFzOnOff + genOnOff/genPowerCfg binds).
// Live STATE DPs decoded here; the SET-only config knobs (DP6
// vibration_sensitivity, DP101 sampling_interval, DP104/105 illuminance_v0/v1,
// DP106 illuminance_calibration) are deferred per convention.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kIllumWarn[]={{0,"none"},{1,"low"},{2,"high"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"occupancy"),                        // z2m trueFalse1
    ::zhc::tuya::dp::binary(3,"vibration"),                        // z2m raw -> binary
    ::zhc::tuya::dp::numeric(4,"battery",1),                       // z2m raw (%)
    ::zhc::tuya::dp::numeric(20,"illuminance",1),                  // z2m raw (lx)
    ::zhc::tuya::dp::numeric(50,"vibration_count",1),              // z2m raw (times)
    ::zhc::tuya::dp::enum_lookup(107,"illuminance_warning",kIllumWarn,3)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,6}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;  // read-only sensor: fz decode only
constexpr const char* kM[]={"Excellux"};
constexpr const char* kN[]={"PIRIV01"};
constexpr const char* kWarnOpts[]={"none","low","high"};
constexpr Expose kExp[]={
    {"occupancy",           ExposeType::Binary,  Access::State, nullptr, "Presence state (true: motion detected)", nullptr, 0},
    {"vibration",           ExposeType::Binary,  Access::State, nullptr, "Vibration state (true: vibration detected)", nullptr, 0},
    {"battery",             ExposeType::Numeric, Access::State, "%",     "Battery",              nullptr, 0},
    {"illuminance",         ExposeType::Numeric, Access::State, "lx",    "Illuminance",          nullptr, 0},
    {"vibration_count",     ExposeType::Numeric, Access::State, "times", "Vibration count",      nullptr, 0},
    {"illuminance_warning", ExposeType::Enum,    Access::State, nullptr, "Illuminance warning level", kWarnOpts, 3}};
}
extern const PreparedDefinition kDef_ZG_104PLV{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="ZG-104PLV",
    .vendor="Excellux",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
