// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya ZY-N1 sound level sensor (z2m v26.115.0, #13332).
// DP101 fans out to `noise_state` (10 labels) and `noise_detected` (true on
// states 0, 2, 3) — two rows on one datapoint, the second kTuyaDpFlagEnumBool.
// z2m-source: tuya.ts #ZY-N1.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kNoiseStatus8[] = { {0,"no_noise"}, {1,"noise_normal"}, {2,"noise"} };
constexpr ::zhc::tuya::TuyaEnumEntry kReportMode13[] = { {0,"collect_noise_floor"}, {1,"realtime"}, {2,"threshold"} };
constexpr ::zhc::tuya::TuyaEnumEntry kNoiseState101[] = { {0,"noise"}, {1,"no_noise"}, {2,"noise_2min"}, {3,"noise_5min"}, {4,"no_noise_2min"}, {5,"no_noise_5min"}, {6,"no_noise_10min"}, {7,"noise_normal"}, {8,"noise_normal_2min"}, {9,"noise_normal_5min"} };
constexpr ::zhc::tuya::TuyaEnumEntry kReportThreshold102[] = { {0,"1_db"}, {1,"3_db"}, {2,"5_db"}, {3,"10_db"}, {4,"20_db"}, {5,"no_report"} };
constexpr ::zhc::tuya::TuyaEnumEntry kNoiseDetected[] = { {0,"noise"}, {2,"noise_2min"}, {3,"noise_5min"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "noise", 1),
        ::zhc::tuya::dp::enum_lookup(8, "noise_status", kNoiseStatus8, 3),
        ::zhc::tuya::dp::enum_lookup(13, "report_mode", kReportMode13, 3),
        ::zhc::tuya::dp::numeric(16, "noise_lower_limit", 1),
        ::zhc::tuya::dp::numeric(18, "collect_time", 1),
        ::zhc::tuya::dp::numeric(20, "noise_upper_limit", 1),
        ::zhc::tuya::dp::numeric(22, "noise_hold_time", 1),
        ::zhc::tuya::dp::binary(23, "indicator"),
        ::zhc::tuya::dp::enum_lookup(101, "noise_state", kNoiseState101, 10),
        { 101, "noise_detected", ::zhc::TuyaDpType::Enum, 1, kNoiseDetected, 3, ::zhc::tuya::kTuyaDpFlagEnumBool },
        ::zhc::tuya::dp::enum_lookup(102, "report_threshold", kReportThreshold102, 6),
        ::zhc::tuya::dp::numeric(103, "noise_delay", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts2[] = { "no_noise", "noise_normal", "noise" };
constexpr const char* kOpts3[] = { "noise", "no_noise", "noise_2min", "noise_5min", "no_noise_2min", "no_noise_5min", "no_noise_10min", "noise_normal", "noise_normal_2min", "noise_normal_5min" };
constexpr const char* kOpts4[] = { "collect_noise_floor", "realtime", "threshold" };
constexpr const char* kOpts10[] = { "1_db", "3_db", "5_db", "10_db", "20_db", "no_report" };
constexpr Expose kExp[] = {
    {"noise", ExposeType::Numeric, Access::State, "dB", "Measured sound level, reported according to report_threshold", nullptr, 0},
    {"noise_detected", ExposeType::Binary, Access::State, nullptr, "Noise above noise_upper_limit detected (respects noise_delay and noise_hold_time)", nullptr, 0},
    {"noise_status", ExposeType::Enum, Access::State, nullptr, "Current level: below noise_lower_limit, between the limits, or above noise_upper_limit", kOpts2, 3},
    {"noise_state", ExposeType::Enum, Access::State, nullptr, "Debounced noise state including its duration", kOpts3, 10},
    {"report_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts4, 3},
    {"noise_lower_limit", ExposeType::Numeric, Access::StateSet, "dB", "Below this level the status is no_noise", nullptr, 0, ExposeCategory::State, 0, 60, 1},
    {"noise_upper_limit", ExposeType::Numeric, Access::StateSet, "dB", "Above this level the status is noise", nullptr, 0, ExposeCategory::State, 10, 100, 1},
    {"collect_time", ExposeType::Numeric, Access::StateSet, "s", "Duration of the collect_noise_floor and realtime modes", nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"noise_hold_time", ExposeType::Numeric, Access::StateSet, "s", "How long noise is held after the level drops", nullptr, 0, ExposeCategory::State, 0, 300, 1},
    {"noise_delay", ExposeType::Numeric, Access::StateSet, "s", "How long the level must stay above noise_upper_limit before noise is reported", nullptr, 0, ExposeCategory::State, 0, 300, 1},
    {"report_threshold", ExposeType::Enum, Access::StateSet, nullptr, "Report the sound level when it changes by this amount", kOpts10, 6},
    {"indicator", ExposeType::Binary, Access::StateSet, nullptr, "LED indicator", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_r6kfl9ta" };
}  // namespace

extern const PreparedDefinition kDef_ZY_N1{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZY-N1",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
