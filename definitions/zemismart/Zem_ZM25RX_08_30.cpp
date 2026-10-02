// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Zemismart ZM25RX-08/30 tubular motor (TS0601 / _TZE200_7eue9vhc,
// _TZE200_bv1jcqqu, _TZE200_wehza30a), z2m v26.115.1 parity.
//
// Replaces three identical per-manufacturer copies that carried only position,
// battery and motor_direction — with the label "reverse" where upstream says
// "reversed" — and no cover action at all. Upstream's full table:
//   DP1 state {OPEN: 0, STOP: 1, CLOSE: 2}, DP2/3 position, DP5 motor_direction,
//   DP7 motor_state {opening: 0, closing: 1, stopped: 2} — `stopped` is z2m
//   v26.113.0 (#13293), DP13/103 battery, DP101 program / click_control
//   (set-only step commands sharing one datapoint).
// Upstream's invert_cover option swaps the tables; ZHC has no per-device
// options, so the non-inverted tables are ported.
// z2m-source: zemismart.ts #ZM25RX-08/30.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::zemismart {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kAction[]  = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]     = { {0,"normal"}, {1,"reversed"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMotor[]   = { {0,"opening"}, {1,"closing"}, {2,"stopped"} };
constexpr ::zhc::tuya::TuyaEnumEntry kProgram[] = { {1,"set_bottom"}, {0,"set_upper"}, {4,"reset"} };
constexpr ::zhc::tuya::TuyaEnumEntry kClick[]   = { {3,"lower"}, {2,"upper"}, {6,"lower_micro"}, {5,"upper_micro"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kAction, 3),
        ::zhc::tuya::dp::numeric(2, "position", 1),
        ::zhc::tuya::dp::numeric(3, "position", 1),
        ::zhc::tuya::dp::enum_lookup(5, "motor_direction", kDir, 2),
        ::zhc::tuya::dp::enum_lookup(7, "motor_state", kMotor, 3),
        ::zhc::tuya::dp::numeric(13, "battery", 1),
        ::zhc::tuya::dp::enum_lookup(101, "program", kProgram, 3),
        ::zhc::tuya::dp::enum_lookup(101, "click_control", kClick, 4),
        ::zhc::tuya::dp::numeric(103, "battery", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kActionOpts[]  = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kDirOpts[]     = { "normal", "reversed" };
constexpr const char* kMotorOpts[]   = { "opening", "closing", "stopped" };
constexpr const char* kProgramOpts[] = { "set_bottom", "set_upper", "reset" };
constexpr const char* kClickOpts[]   = { "upper", "upper_micro", "lower", "lower_micro" };
constexpr Expose kExp[] = {
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kActionOpts, 3},
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0},
    {"motor_state",     ExposeType::Enum,    Access::State,    nullptr, "Motor state", kMotorOpts, 3},
    {"battery",         ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"program",         ExposeType::Enum,    Access::Set,      nullptr, "Set the upper/bottom limit", kProgramOpts, 3},
    {"click_control",   ExposeType::Enum,    Access::Set,      nullptr,
     "Control motor in steps (ignores set limits; normal/micro = 120deg/5deg movement)", kClickOpts, 4},
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, "Motor direction", kDirOpts, 2},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_7eue9vhc", "_TZE200_bv1jcqqu", "_TZE200_wehza30a" };
}  // namespace

extern const PreparedDefinition kDef_ZM25RX_08_30{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="ZM25RX-08/30",
    .vendor="Zemismart",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::zemismart
