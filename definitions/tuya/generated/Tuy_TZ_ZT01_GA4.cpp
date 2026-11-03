// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TZ-ZT01_GA4 temperature & humidity sensor with external probe.
// Was a generated min stub (genPowerCfg / genOnOff, no datapoints) that decoded
// nothing; z2m v26.125.0 window ports the tuyaBase({dp: true}) map:
// DP1 temperature /10, DP2 humidity, DP3 battery_state, DP38 temperature_probe /10.
// `_TZE2841000000_8se38w3c` reaches this def through the twin-prefix retry.
// z2m-source: tuya.ts #TZ-ZT01_GA4.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kBatteryState[] = { {0, "low"}, {1, "medium"}, {2, "high"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "temperature", 10),
        ::zhc::tuya::dp::numeric(2, "humidity", 1),
        ::zhc::tuya::dp::enum_lookup(3, "battery_state", kBatteryState, 3),
        ::zhc::tuya::dp::numeric(38, "temperature_probe", 10),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kBatteryStateOpts[] = { "low", "medium", "high" };
constexpr Expose kExp[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"temperature_probe", ExposeType::Numeric, Access::State, "°C", "Probe temperature", nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, "State of the battery", kBatteryStateOpts, 3,
     ExposeCategory::Diagnostic},
};
constexpr const char* kModels_TZ_ZT01_GA4[] = { "TS0601" };
constexpr const char* kManus_TZ_ZT01_GA4[] = { "_TZE284_8se38w3c" };
}  // namespace

extern const PreparedDefinition kDef_TZ_ZT01_GA4{
    .zigbee_models=kModels_TZ_ZT01_GA4, .zigbee_models_count=sizeof(kModels_TZ_ZT01_GA4)/sizeof(kModels_TZ_ZT01_GA4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TZ_ZT01_GA4, .manufacturer_names_count=sizeof(kManus_TZ_ZT01_GA4)/sizeof(kManus_TZ_ZT01_GA4[0]),
    .model="TZ-ZT01_GA4", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
