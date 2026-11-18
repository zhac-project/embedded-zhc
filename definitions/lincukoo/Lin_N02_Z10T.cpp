// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Lincukoo N02-Z10T noise sensor, TS0601 _TZE284/_TZE2841000000_yfttaycl
// (z2m v26.133.0 window). DPs: 1 sound_value dB, 8 action {low, high},
// 12 battery, 14 / 21 / 22 noise report / change thresholds and hold time,
// 23 indicator. All numbers raw.
// z2m-source: lincukoo.ts #N02-Z10T.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::lincukoo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kAction[] = { {0, "low"}, {1, "high"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "sound_value"),
        ::zhc::tuya::dp::action(8, kAction, 2),
        ::zhc::tuya::dp::numeric(12, "battery"),
        ::zhc::tuya::dp::numeric(14, "noise_report_threshold"),
        ::zhc::tuya::dp::numeric(21, "noise_change_threshold"),
        ::zhc::tuya::dp::numeric(22, "noise_hold_time"),
        ::zhc::tuya::dp::binary(23, "indicator"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kActionOpts[] = { "low", "high" };
constexpr Expose kExp[] = {
    {"sound_value", ExposeType::Numeric, Access::State, "dB", "Current measured sound level", nullptr, 0,
     ExposeCategory::State, 60, 130, 1},
    {"action", ExposeType::Enum, Access::State, nullptr, "Noise status, high = above report threshold", kActionOpts, 2},
    {"battery", ExposeType::Numeric, Access::State, "%", "Remaining battery in %", nullptr, 0,
     ExposeCategory::Diagnostic, 0, 100, 1},
    {"noise_report_threshold", ExposeType::Numeric, Access::StateSet, "dB",
     "Sound level that triggers a high-noise report", nullptr, 0, ExposeCategory::State, 80, 130, 1},
    {"noise_change_threshold", ExposeType::Numeric, Access::StateSet, "dB",
     "Minimum sound level change that triggers an extra report", nullptr, 0, ExposeCategory::State, 5, 20, 1},
    {"noise_hold_time", ExposeType::Numeric, Access::StateSet, "s",
     "How long the level must stay above threshold before reporting", nullptr, 0, ExposeCategory::State, 10, 300, 1},
    {"indicator", ExposeType::Binary, Access::StateSet, nullptr, "Indicator LED", nullptr, 0},
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE2841000000_yfttaycl", "_TZE284_yfttaycl" };

}  // namespace

extern const PreparedDefinition kDef_N02_Z10T{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]), .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN, .manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="N02-Z10T", .vendor="Lincukoo", .meta=nullptr,
    .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::lincukoo
