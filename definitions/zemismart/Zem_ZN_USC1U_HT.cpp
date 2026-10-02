// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Zemismart ZN-USC1U-HT smart curtain wall switch (TS0601 /
// _TZE204_mpg22jc1), z2m v26.115.1 parity. Graduated from
// zemismart/generated/Zem__TZE204_mpg22jc1.cpp.
//
// z2m v26.111.0 (#13207): DP8 `motor_steering` FORWARD/BACKWARD is now
// `motor_direction` (tubularMotorDirection {normal: 0, reversed: 1}). The cover
// action is coverAction {OPEN, STOP, CLOSE, CONTINUE}.
// z2m-source: zemismart.ts #ZN-USC1U-HT.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::zemismart {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kAction[] = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"}, {3,"CONTINUE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]    = { {0,"normal"}, {1,"reversed"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kAction, 4),
        ::zhc::tuya::dp::numeric(2, "position", 1),
        ::zhc::tuya::dp::enum_lookup(8, "motor_direction", kDir, 2),
        ::zhc::tuya::dp::numeric(10, "calibration_time", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kActionOpts[] = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kDirOpts[]    = { "normal", "reversed" };
constexpr Expose kExp[] = {
    {"state",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kActionOpts, 3},
    {"position",         ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0},
    {"motor_direction",  ExposeType::Enum,    Access::StateSet, nullptr, "Motor direction", kDirOpts, 2},
    {"calibration_time", ExposeType::Numeric, Access::StateSet, "s",
     "Calibration time in seconds (Please fully close the curtain before set the calibration time)", nullptr, 0,
     ExposeCategory::State, 0, 500, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_mpg22jc1" };
}  // namespace

extern const PreparedDefinition kDef_ZN_USC1U_HT{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="ZN-USC1U-HT",
    .vendor="Zemismart",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::zemismart
