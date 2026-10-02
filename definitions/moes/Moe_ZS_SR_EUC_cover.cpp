// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Moes ZS-SR-EUC "Star ring" smart curtain switch (TS0601 /
// _TZE204_srmahpwl), z2m v26.115.1 parity.
//
// Graduated from three generated copies of the same device: Moes__TZE204_
// srmahpwl (registered, DP map with motor_steering), Moe_ZS_SR_EUC (registered,
// a ZCL cover stub that a datapoint device never feeds) and Gen__TZE204_srmahpwl
// (unregistered). z2m v26.111.0 (#13207): DP8 `motor_steering` FORWARD/BACKWARD
// is now `motor_direction` (tubularMotorDirection {normal: 0, reversed: 1}).
// DP2 keeps upstream's overshoot workaround: above 150 reads 0, 101..150 reads
// 100 (the motor keeps counting past its end stops).
// z2m-source: moes.ts #ZS-SR-EUC.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::moes {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kAction[] = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"}, {3,"CONTINUE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCalib[]  = { {0,"START"}, {1,"END"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]    = { {0,"normal"}, {1,"reversed"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kAction, 4),
        ::zhc::tuya::dp::position_overflow(2, "position"),
        ::zhc::tuya::dp::enum_lookup(3, "calibration", kCalib, 2),
        ::zhc::tuya::dp::enum_lookup(8, "motor_direction", kDir, 2),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kActionOpts[] = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kCalibOpts[]  = { "START", "END" };
constexpr const char* kDirOpts[]    = { "normal", "reversed" };
constexpr Expose kExp[] = {
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kActionOpts, 3},
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0},
    {"calibration",     ExposeType::Enum,    Access::StateSet, nullptr, "Calibration", kCalibOpts, 2},
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, "Motor direction", kDirOpts, 2},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_srmahpwl" };
}  // namespace

extern const PreparedDefinition kDef_ZS_SR_EUC_cover{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="ZS-SR-EUC",
    .vendor="Moes",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::moes
