// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0301_cover_1 cover motor (TS0301 / _TZE210_m6lwazh9).
//
// Graduated 2026-09-04 out of tuya/generated/ to wire dp13 `battery`
// (z2m v26.105.0, #13092). Tables from lib/tuya.ts:
//   coverAction   {OPEN:0, STOP:1, CLOSE:2, CONTINUE:3}
//   dp2 + dp3 both carry "position" (identity); ZHC fires every matching row.
//   dp5 reverse_direction {forward:0, back:1}
// z2m-source: tuya.ts #TS0301_cover_1.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kAction[] = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"}, {3,"CONTINUE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]    = { {0,"forward"}, {1,"back"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kAction, 4),
        ::zhc::tuya::dp::numeric(2, "position", 1),
        ::zhc::tuya::dp::numeric(3, "position", 1),
        ::zhc::tuya::dp::enum_lookup(5, "reverse_direction", kDir, 2),
        ::zhc::tuya::dp::numeric(13, "battery", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kActionOpts[] = { "OPEN", "STOP", "CLOSE", "CONTINUE" };
constexpr const char* kDirOpts[]    = { "forward", "back" };
constexpr Expose kExp[] = {
    {"state",             ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover action",   kActionOpts, 4},
    {"position",          ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover position", nullptr,     0},
    {"reverse_direction", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Reverse the motor direction", kDirOpts, 2, ExposeCategory::Config},
    {"battery",           ExposeType::Numeric, ::zhc::Access::State,    "%",     "Battery percentage", nullptr, 0},
};
constexpr const char* kM[] = { "TS0301" };
constexpr const char* kN[] = { "_TZE210_m6lwazh9" };
}  // namespace

extern const PreparedDefinition kDef_TS0301_cover_1{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0301_cover_1",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
