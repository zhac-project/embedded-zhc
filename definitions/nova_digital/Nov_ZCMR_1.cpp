// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital ZCMR-1 roller blind motor (TS0601 / _TZE204_dqy15zxy;
// z2m v26.104.0, #13078).
//
// dp2 and dp3 both carry "position" upstream (identity coverPosition); ZHC
// fires every matching row. `border` is write-only and upstream exposes only
// up/down of the five values the lookup knows -- the full table is kept for
// decode. queryOnConfigure is the standard Tuya base configure.
// z2m-source: nova_digital.ts #ZCMR-1.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::nova_digital {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kAction[]    = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]       = { {0,"forward"}, {1,"back"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSituation[] = { {0,"fully_open"}, {1,"fully_close"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBorder[]    = { {0,"up"}, {1,"down"}, {2,"up_delete"}, {3,"down_delete"}, {4,"remove_top_bottom"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kAction, 3),
        ::zhc::tuya::dp::numeric(2, "position", 1),
        ::zhc::tuya::dp::numeric(3, "position", 1),
        ::zhc::tuya::dp::enum_lookup(5, "reverse_direction", kDir, 2),
        ::zhc::tuya::dp::numeric(10, "travel_time", 1),
        ::zhc::tuya::dp::enum_lookup(11, "situation_set", kSituation, 2),
        ::zhc::tuya::dp::enum_lookup(16, "border", kBorder, 5),
        ::zhc::tuya::dp::numeric(19, "position_best", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kActionOpts[]    = { "OPEN", "STOP", "CLOSE" };
constexpr const char* kDirOpts[]       = { "forward", "back" };
constexpr const char* kSituationOpts[] = { "fully_open", "fully_close" };
constexpr const char* kBorderOpts[]    = { "up", "down" };
constexpr Expose kExp[] = {
    {"state",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover action",   kActionOpts, 3},
    {"position", ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover position", nullptr, 0},
    {"reverse_direction", ExposeType::Enum, ::zhc::Access::StateSet, nullptr,
     "Reverse the motor direction. This resets the upper and lower limits; recalibrate them using border afterwards.",
     kDirOpts, 2, ExposeCategory::Config},
    {"border", ExposeType::Enum, ::zhc::Access::Set, nullptr, "Store the current position as the upper or lower limit", kBorderOpts, 2, ExposeCategory::Config},
    {"situation_set", ExposeType::Enum, ::zhc::Access::State, nullptr, "Indicates whether 100% represents fully open or fully closed", kSituationOpts, 2, ExposeCategory::Diagnostic},
    {"travel_time",   ExposeType::Numeric, ::zhc::Access::State, "ms", "Total calibrated travel time", nullptr, 0, ExposeCategory::Diagnostic},
    {"position_best", ExposeType::Numeric, ::zhc::Access::State, "%",  "Preset position stored by the motor", nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_dqy15zxy" };
}  // namespace

extern const PreparedDefinition kDef_ZCMR_1{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZCMR-1",.vendor="Nova Digital",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::nova_digital
