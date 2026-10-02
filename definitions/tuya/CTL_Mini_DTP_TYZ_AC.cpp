// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: CTL CTL-Mini-DTP-TYZ/AC radar + PIR dual-technology presence sensor
// (z2m v26.113.0, #13296). DP119 `system_info` is a string datapoint, which this
// port does not decode (no string DP surface); everything else is mapped.
// z2m-source: tuya.ts #CTL-Mini-DTP-TYZ/AC.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kLightLinkage106[] = { {0,"ON"}, {1,"OFF"} };
constexpr ::zhc::tuya::TuyaEnumEntry kIlluminanceLinkage107[] = { {0,"ON"}, {1,"OFF"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBreathingIndicator108[] = { {0,"ON"}, {1,"OFF"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDetectionMethod109[] = { {0,"presence"}, {1,"motion"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSensitivity110[] = { {0,"LL"}, {1,"L"}, {2,"M"}, {3,"H"}, {4,"HH"} };
constexpr ::zhc::tuya::TuyaEnumEntry kProgramFunction111[] = { {0,"null"}, {1,"no_one_1min"}, {2,"no_one_3min"}, {3,"no_one_5min"}, {4,"no_one_10min"}, {5,"no_one_15min"}, {6,"no_one_30min"}, {7,"no_one_1hour"}, {8,"no_one_2hour"}, {9,"no_one_4hour"}, {10,"no_one_8hour"}, {11,"no_one_12hour"}, {12,"no_one_24hour"}, {13,"presence_1min"}, {14,"presence_3min"}, {15,"presence_5min"}, {16,"presence_10min"}, {17,"presence_15min"}, {18,"presence_30min"}, {19,"presence_1hour"}, {20,"presence_2hour"}, {21,"presence_4hour"}, {22,"presence_8hour"}, {23,"presence_12hour"}, {24,"presence_24hour"} };
constexpr ::zhc::tuya::TuyaEnumEntry kInstallationMode112[] = { {0,"ceiling"}, {1,"wall"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnvironmentLearning115[] = { {0,"none"}, {1,"light"}, {2,"medium"}, {3,"deep"}, {4,"disable"} };
constexpr ::zhc::tuya::TuyaEnumEntry kManualAnnotation116[] = { {0,"null"}, {1,"no_one_last_1h"}, {2,"no_one_last_2h"}, {3,"no_one_last_4h"}, {4,"no_one_last_8h"}, {5,"no_one_last_12h"}, {6,"no_one_last_24h"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSensorPower117[] = { {0,"on"}, {1,"off"}, {2,"off_10s_restart"}, {3,"off_30s_restart"}, {4,"off_60s_restart"}, {5,"pause_upload"}, {6,"pause_upload_10s"}, {7,"pause_upload_30s"}, {8,"pause_upload_60s"}, {9,"pause_upload_3min"}, {10,"pause_upload_5min"}, {11,"pause_upload_10min"}, {12,"pause_upload_15min"}, {13,"pause_upload_30min"}, {14,"pause_upload_1hour"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSystemSetting118[] = { {0,"none"}, {1,"identify_start"}, {2,"identify_stop"}, {3,"check_start"}, {4,"check_stop"}, {5,"restore_factory"}, {6,"state_flip_report"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "presence"),
        ::zhc::tuya::dp::numeric(101, "illuminance", 1),
        ::zhc::tuya::dp::numeric(102, "illuminance_threshold_high", 1),
        ::zhc::tuya::dp::numeric(103, "illuminance_threshold_low", 1),
        ::zhc::tuya::dp::numeric(104, "presence_timeout", 1),
        ::zhc::tuya::dp::binary(105, "light_switch"),
        ::zhc::tuya::dp::enum_lookup(106, "light_linkage", kLightLinkage106, 2),
        ::zhc::tuya::dp::enum_lookup(107, "illuminance_linkage", kIlluminanceLinkage107, 2),
        ::zhc::tuya::dp::enum_lookup(108, "breathing_indicator", kBreathingIndicator108, 2),
        ::zhc::tuya::dp::enum_lookup(109, "detection_method", kDetectionMethod109, 2),
        ::zhc::tuya::dp::enum_lookup(110, "sensitivity", kSensitivity110, 5),
        ::zhc::tuya::dp::enum_lookup(111, "program_function", kProgramFunction111, 25),
        ::zhc::tuya::dp::enum_lookup(112, "installation_mode", kInstallationMode112, 2),
        ::zhc::tuya::dp::numeric(113, "installation_height", 10),
        ::zhc::tuya::dp::numeric(114, "detection_radius", 10),
        ::zhc::tuya::dp::enum_lookup(115, "environment_learning", kEnvironmentLearning115, 5),
        ::zhc::tuya::dp::enum_lookup(116, "manual_annotation", kManualAnnotation116, 7),
        ::zhc::tuya::dp::enum_lookup(117, "sensor_power", kSensorPower117, 15),
        ::zhc::tuya::dp::enum_lookup(118, "system_setting", kSystemSetting118, 7),
        ::zhc::tuya::dp::numeric(120, "detection_range", 10),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts6[] = { "ON", "OFF" };
constexpr const char* kOpts7[] = { "ON", "OFF" };
constexpr const char* kOpts8[] = { "ON", "OFF" };
constexpr const char* kOpts9[] = { "presence", "motion" };
constexpr const char* kOpts10[] = { "LL", "L", "M", "H", "HH" };
constexpr const char* kOpts11[] = { "ceiling", "wall" };
constexpr const char* kOpts15[] = { "none", "light", "medium", "deep", "disable" };
constexpr const char* kOpts16[] = { "on", "off", "off_10s_restart", "off_30s_restart", "off_60s_restart", "pause_upload", "pause_upload_10s", "pause_upload_30s", "pause_upload_60s", "pause_upload_3min", "pause_upload_5min", "pause_upload_10min", "pause_upload_15min", "pause_upload_30min", "pause_upload_1hour" };
constexpr const char* kOpts17[] = { "none", "identify_start", "identify_stop", "check_start", "check_stop", "restore_factory", "state_flip_report" };
constexpr Expose kExp[] = {
    {"presence", ExposeType::Binary, Access::State, nullptr, "Presence detected by the radar/PIR sensor", nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"illuminance_threshold_high", ExposeType::Numeric, Access::StateSet, "lx", "Turn the linked light off above this illuminance", nullptr, 0, ExposeCategory::Config, 0, 2000, 1},
    {"illuminance_threshold_low", ExposeType::Numeric, Access::StateSet, "lx", "Allow presence to turn the linked light on below this illuminance", nullptr, 0, ExposeCategory::Config, 0, 1000, 1},
    {"presence_timeout", ExposeType::Numeric, Access::StateSet, "s", "Delay before reporting no presence", nullptr, 0, ExposeCategory::Config, 5, 3600, 1},
    {"light_switch", ExposeType::Binary, Access::StateSet, nullptr, "Light output state", nullptr, 0},
    {"light_linkage", ExposeType::Enum, Access::StateSet, nullptr, "Enable automatic light linkage", kOpts6, 2, ExposeCategory::Config},
    {"illuminance_linkage", ExposeType::Enum, Access::StateSet, nullptr, "Apply the illuminance thresholds to light linkage", kOpts7, 2, ExposeCategory::Config},
    {"breathing_indicator", ExposeType::Enum, Access::StateSet, nullptr, "Enable the breathing indicator while presence is detected", kOpts8, 2, ExposeCategory::Config},
    {"detection_method", ExposeType::Enum, Access::StateSet, nullptr, "Motion-only or continuous presence detection", kOpts9, 2, ExposeCategory::Config},
    {"sensitivity", ExposeType::Enum, Access::StateSet, nullptr, "LL: very low, L: low, M: medium, H: high, HH: very high", kOpts10, 5, ExposeCategory::Config},
    {"installation_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts11, 2, ExposeCategory::Config},
    {"installation_height", ExposeType::Numeric, Access::StateSet, "m", nullptr, nullptr, 0, ExposeCategory::Config},
    {"detection_radius", ExposeType::Numeric, Access::StateSet, "m", "Ceiling detection radius", nullptr, 0, ExposeCategory::Config},
    {"detection_range", ExposeType::Numeric, Access::StateSet, "m", "Wall detection distance", nullptr, 0, ExposeCategory::Config},
    {"environment_learning", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts15, 5, ExposeCategory::Config},
    {"sensor_power", ExposeType::Enum, Access::StateSet, nullptr, "Power-cycle the sensor or pause its reports", kOpts16, 15, ExposeCategory::Config},
    {"system_setting", ExposeType::Enum, Access::StateSet, nullptr, "Warning: restore_factory resets the device to factory settings", kOpts17, 7, ExposeCategory::Config},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_5qfrnbqs" };
}  // namespace

extern const PreparedDefinition kDef_CTL_Mini_DTP_TYZ_AC{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="CTL-Mini-DTP-TYZ/AC",.vendor="CTL",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
