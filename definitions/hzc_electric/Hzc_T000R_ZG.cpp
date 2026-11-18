// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HZC Electric T000R_ZG thermostat (new in z2m v26.126.0..v26.133.0).
// m.thermostat (local temperature, heating setpoint 5..40, system_mode heat/off,
// local_temperature_calibration -9..9 in 0.1 °C int8) + m.battery (percentage).
// Custom attributes, all reported 0..1 h upstream:
//   hzcWindow 0xE000   0x0000 window_check (Bool, writable), 0x0001 window_status
//   hzcValue  0xE001   0x0001 value_switch, 0x0002 control_type {PDPID, NF},
//                      0x0003 value_opening (/10), 0x0004 scale_switch,
//                      0x0005 boost_switch, 0x0006 boost_countdown (1..90),
//                      0x0007 work_mode {manual … off}
//   hvacThermostat     0x8001 frost (Bool), 0x9000 frost_temp (u8, /10 °C)
//   hvacUserInterface  0x0001 keypad_lockout (Bool on keypadLockout),
//                      0x8000 display_mode {forward, reverse}, 0x8002
//                      holiday_time (d), 0x8004 auto_time (Bool)
// 0xE001 is labelled "manuSpecificTuya2" in zhc/cluster_names.hpp (cosmetic).
// 0xE000 has no name there, so its frames reach every converter of the def
// ("fail open"): window_status (attr 1) would also land in keypad_lockout and
// value_switch. gated() keeps the named maps off those frames and the hzcWindow
// map on them only.
// Not ported: the configure-time clock write (hvacThermostat 0x800B UINT32, local
// seconds since 1970: needs a configure step that reads the hub clock); meta
// timeout; the battery percentage reporting row (genPowerCfg bound only).
// z2m-source: hzc_electric.ts #T000R_ZG.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hzc_electric {
namespace {

namespace g = ::zhc::generic;

// A named wrapper skips frames of unnamed clusters; an unnamed one (cluster
// nullptr) takes only those, non-manufacturer-specific.
// ponytail: copied per def (also heatzy, edflab, repenic_ltd); hoist into _generic
// if more defs need it.
bool fz_gated(const DecodedMessage& msg, const FzConverter& self, const PreparedDefinition& def,
              RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const bool named = msg.cluster != nullptr;
    if (self.cluster ? !named : (named || msg.manufacturer_specific)) return false;
    const auto* inner = static_cast<const FzConverter*>(self.user_config);
    return inner->fn.zcl_fn(msg, *inner, def, ctx, out);
}
constexpr FzConverter gated(const char* cluster, const FzConverter* inner) {
    FzConverter c = g::zcl_attr_fz(cluster, nullptr);
    c.fn.zcl_fn = fz_gated;
    c.user_config = inner;
    return c;
}

constexpr std::uint8_t kBool = g::kZclAttrFlagBool;

// hvacThermostat extras.
constexpr g::ZclAttrRow kThermRows[] = {
    { 0x0010, "local_temperature_calibration", 10 },
    { 0x8001, "frost", 1, nullptr, 0, kBool },
    { 0x9000, "frost_temp", 10 },
};
constexpr g::ZclAttrMap kThermMap{ kThermRows, sizeof(kThermRows)/sizeof(kThermRows[0]) };
constexpr FzConverter kFzThermRaw = g::zcl_attr_fz("hvacThermostat", &kThermMap);
constexpr FzConverter kFzTherm = gated("hvacThermostat", &kFzThermRaw);

// hvacUserInterfaceCfg.
constexpr g::ZclWriteLookup kDisplayModes[] = { {"forward", 0}, {"reverse", 1} };
constexpr g::ZclAttrRow kUiRows[] = {
    { 0x0001, "keypad_lockout", 1, nullptr, 0, kBool },
    { 0x8000, "display_mode", 1, kDisplayModes, sizeof(kDisplayModes)/sizeof(kDisplayModes[0]) },
    { 0x8002, "holiday_time" },
    { 0x8004, "auto_time", 1, nullptr, 0, kBool },
};
constexpr g::ZclAttrMap kUiMap{ kUiRows, sizeof(kUiRows)/sizeof(kUiRows[0]) };
constexpr FzConverter kFzUiRaw = g::zcl_attr_fz("hvacUserInterfaceCfg", &kUiMap);
constexpr FzConverter kFzUi = gated("hvacUserInterfaceCfg", &kFzUiRaw);

// hzcValue 0xE001 (named "manuSpecificTuya2" here).
constexpr g::ZclWriteLookup kControlTypes[] = { {"PDPID", 0}, {"NF", 1} };
constexpr g::ZclWriteLookup kWorkModes[] = {
    {"manual", 0}, {"program", 1}, {"holiday", 2}, {"comfortable", 3}, {"eco", 4}, {"off", 5} };
constexpr g::ZclAttrRow kValueRows[] = {
    { 0x0001, "value_switch", 1, nullptr, 0, kBool },
    { 0x0002, "control_type", 1, kControlTypes, sizeof(kControlTypes)/sizeof(kControlTypes[0]) },
    { 0x0003, "value_opening", 10 },
    { 0x0004, "scale_switch", 1, nullptr, 0, kBool },
    { 0x0005, "boost_switch", 1, nullptr, 0, kBool },
    { 0x0006, "boost_countdown" },
    { 0x0007, "work_mode", 1, kWorkModes, sizeof(kWorkModes)/sizeof(kWorkModes[0]) },
};
constexpr g::ZclAttrMap kValueMap{ kValueRows, sizeof(kValueRows)/sizeof(kValueRows[0]) };
constexpr FzConverter kFzValueRaw = g::zcl_attr_fz("manuSpecificTuya2", &kValueMap);
constexpr FzConverter kFzValue = gated("manuSpecificTuya2", &kFzValueRaw);

// hzcWindow 0xE000 (unnamed).
constexpr g::ZclAttrRow kWindowRows[] = {
    { 0x0000, "window_check", 1, nullptr, 0, kBool },
    { 0x0001, "window_status", 1, nullptr, 0, kBool },
};
constexpr g::ZclAttrMap kWindowMap{ kWindowRows, sizeof(kWindowRows)/sizeof(kWindowRows[0]) };
constexpr FzConverter kFzWindowRaw = g::zcl_attr_fz(nullptr, &kWindowMap);
constexpr FzConverter kFzWindow = gated(nullptr, &kFzWindowRaw);

// Writes. Scaled ones are refused outside the attribute's wire range.
constexpr g::ZclWriteSpec kCalibrationSpec{ "local_temperature_calibration", 0x0010, 0x28, 0, nullptr, 0, 0, 10, -90, 90 };
constexpr g::ZclWriteSpec kFrostSpec{ "frost", 0x8001, 0x10, 0, nullptr, 0 };
constexpr g::ZclWriteSpec kFrostTempSpec{ "frost_temp", 0x9000, 0x20, 0, nullptr, 0, 0, 10, 0, 255 };
constexpr g::ZclWriteSpec kKeypadSpec{ "keypad_lockout", 0x0001, 0x30, 0, nullptr, 0 };
constexpr g::ZclWriteSpec kDisplayModeSpec{ "display_mode", 0x8000, 0x30, 0, kDisplayModes, sizeof(kDisplayModes)/sizeof(kDisplayModes[0]) };
constexpr g::ZclWriteSpec kHolidayTimeSpec{ "holiday_time", 0x8002, 0x20, 0, nullptr, 0 };
constexpr g::ZclWriteSpec kAutoTimeSpec{ "auto_time", 0x8004, 0x10, 0, nullptr, 0 };
constexpr g::ZclWriteSpec kValueSwitchSpec{ "value_switch", 0x0001, 0x10, 0, nullptr, 0 };
constexpr g::ZclWriteSpec kControlTypeSpec{ "control_type", 0x0002, 0x30, 0, kControlTypes, sizeof(kControlTypes)/sizeof(kControlTypes[0]) };
constexpr g::ZclWriteSpec kValueOpeningSpec{ "value_opening", 0x0003, 0x20, 0, nullptr, 0, 0, 10, 0, 255 };
constexpr g::ZclWriteSpec kScaleSwitchSpec{ "scale_switch", 0x0004, 0x10, 0, nullptr, 0 };
constexpr g::ZclWriteSpec kBoostSwitchSpec{ "boost_switch", 0x0005, 0x10, 0, nullptr, 0 };
constexpr g::ZclWriteSpec kBoostCountdownSpec{ "boost_countdown", 0x0006, 0x20, 0, nullptr, 0 };
constexpr g::ZclWriteSpec kWorkModeSpec{ "work_mode", 0x0007, 0x30, 0, kWorkModes, sizeof(kWorkModes)/sizeof(kWorkModes[0]) };
constexpr g::ZclWriteSpec kWindowCheckSpec{ "window_check", 0x0000, 0x10, 0, nullptr, 0 };
constexpr TzConverter kTzCalibration    = g::zcl_write_tz("hvacThermostat", 0x0201, &kCalibrationSpec);
constexpr TzConverter kTzFrost          = g::zcl_write_tz("hvacThermostat", 0x0201, &kFrostSpec);
constexpr TzConverter kTzFrostTemp      = g::zcl_write_tz("hvacThermostat", 0x0201, &kFrostTempSpec);
constexpr TzConverter kTzKeypad         = g::zcl_write_tz("hvacUserInterfaceCfg", 0x0204, &kKeypadSpec);
constexpr TzConverter kTzDisplayMode    = g::zcl_write_tz("hvacUserInterfaceCfg", 0x0204, &kDisplayModeSpec);
constexpr TzConverter kTzHolidayTime    = g::zcl_write_tz("hvacUserInterfaceCfg", 0x0204, &kHolidayTimeSpec);
constexpr TzConverter kTzAutoTime       = g::zcl_write_tz("hvacUserInterfaceCfg", 0x0204, &kAutoTimeSpec);
constexpr TzConverter kTzValueSwitch    = g::zcl_write_tz("hzcValue", 0xE001, &kValueSwitchSpec);
constexpr TzConverter kTzControlType    = g::zcl_write_tz("hzcValue", 0xE001, &kControlTypeSpec);
constexpr TzConverter kTzValueOpening   = g::zcl_write_tz("hzcValue", 0xE001, &kValueOpeningSpec);
constexpr TzConverter kTzScaleSwitch    = g::zcl_write_tz("hzcValue", 0xE001, &kScaleSwitchSpec);
constexpr TzConverter kTzBoostSwitch    = g::zcl_write_tz("hzcValue", 0xE001, &kBoostSwitchSpec);
constexpr TzConverter kTzBoostCountdown = g::zcl_write_tz("hzcValue", 0xE001, &kBoostCountdownSpec);
constexpr TzConverter kTzWorkMode       = g::zcl_write_tz("hzcValue", 0xE001, &kWorkModeSpec);
constexpr TzConverter kTzWindowCheck    = g::zcl_write_tz("hzcWindow", 0xE000, &kWindowCheckSpec);

const FzConverter* const kFz[] = {
    &g::kFzThermostat,
    &g::kFzBattery,
    &kFzTherm,
    &kFzUi,
    &kFzValue,
    &kFzWindow,
};
const TzConverter* const kTz[] = {
    &g::kTzThermostat,
    &kTzCalibration, &kTzFrost, &kTzFrostTemp,
    &kTzKeypad, &kTzDisplayMode, &kTzHolidayTime, &kTzAutoTime,
    &kTzValueSwitch, &kTzControlType, &kTzValueOpening, &kTzScaleSwitch,
    &kTzBoostSwitch, &kTzBoostCountdown, &kTzWorkMode,
    &kTzWindowCheck,
};

constexpr const char* kSystemModes[] = { "heat", "off" };
constexpr const char* kWorkModeLabels[] = { "manual", "program", "holiday", "comfortable", "eco", "off" };
constexpr const char* kControlTypeLabels[] = { "PDPID", "NF" };
constexpr const char* kDisplayModeLabels[] = { "forward", "reverse" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
     ExposeCategory::State, 5, 40, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModes, 2},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
     ExposeCategory::Config, -9, 9, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"work_mode", ExposeType::Enum, Access::StateSet, nullptr, "Work mode", kWorkModeLabels, 6},
    {"window_status", ExposeType::Binary, Access::State, nullptr,
     "Indicates if an open window is detected", nullptr, 0},
    {"window_check", ExposeType::Binary, Access::StateSet, nullptr,
     "Enable open window detection", nullptr, 0, ExposeCategory::Config},
    {"boost_switch", ExposeType::Binary, Access::StateSet, nullptr, "Boost", nullptr, 0},
    {"boost_countdown", ExposeType::Numeric, Access::StateSet, nullptr, "Boost duration", nullptr, 0,
     ExposeCategory::Config, 1, 90, 0},
    {"value_switch", ExposeType::Binary, Access::StateSet, nullptr, "Value switch", nullptr, 0},
    {"control_type", ExposeType::Enum, Access::StateSet, nullptr, "Control type", kControlTypeLabels, 2,
     ExposeCategory::Config},
    {"value_opening", ExposeType::Numeric, Access::StateSet, nullptr, "Value opening", nullptr, 0,
     ExposeCategory::State, 0, 10, 0},
    {"scale_switch", ExposeType::Binary, Access::StateSet, nullptr, "Scale switch", nullptr, 0,
     ExposeCategory::Config},
    {"frost", ExposeType::Binary, Access::StateSet, nullptr, "Frost protection", nullptr, 0,
     ExposeCategory::Config},
    {"frost_temp", ExposeType::Numeric, Access::StateSet, "°C", "Frost protection temperature", nullptr, 0,
     ExposeCategory::Config, 5, 10, 0},
    {"keypad_lockout", ExposeType::Binary, Access::StateSet, nullptr, "Keypad lockout", nullptr, 0,
     ExposeCategory::Config},
    {"display_mode", ExposeType::Enum, Access::StateSet, nullptr, "Display orientation", kDisplayModeLabels, 2,
     ExposeCategory::Config},
    {"holiday_time", ExposeType::Numeric, Access::StateSet, "d", "Holiday mode duration", nullptr, 0,
     ExposeCategory::Config, 1, 99, 0},
    {"auto_time", ExposeType::Binary, Access::StateSet, nullptr, "Enable automatic time sync", nullptr, 0,
     ExposeCategory::Config},
};

constexpr const char* kM[] = { "T000R_ZG" };
constexpr BindingSpec kBind[] = {
    {1, 0x0201},
    {1, 0x0001},
    {1, 0x0204},
    {1, 0xE000},
    {1, 0xE001},
};
// Upstream reporting: MIN..1_HOUR, change 10 (temperatures), 1 (numbers), 0.
constexpr ReportingSpec kRep[] = {
    {1, 0x0201, 0x0000, 0x29, 0, 3600, 10, 0},  // localTemp
    {1, 0x0201, 0x0012, 0x29, 0, 3600, 10, 0},  // occupiedHeatingSetpoint
    {1, 0x0201, 0x001C, 0x30, 0, 3600, 0, 0},   // systemMode
    {1, 0x0201, 0x8001, 0x10, 0, 3600, 0, 0},   // frost
    {1, 0x0201, 0x9000, 0x20, 0, 3600, 1, 0},   // frost_temp
    {1, 0x0204, 0x0001, 0x30, 0, 3600, 0, 0},   // keypadLockout
    {1, 0x0204, 0x8000, 0x30, 0, 3600, 0, 0},   // display_mode
    {1, 0x0204, 0x8002, 0x20, 0, 3600, 1, 0},   // holiday_time
    {1, 0x0204, 0x8004, 0x10, 0, 3600, 0, 0},   // auto_time
    {1, 0xE000, 0x0000, 0x10, 0, 3600, 0, 0},   // windowCheck
    {1, 0xE000, 0x0001, 0x10, 0, 3600, 0, 0},   // windowStatus
    {1, 0xE001, 0x0001, 0x10, 0, 3600, 0, 0},   // valueSwitch
    {1, 0xE001, 0x0002, 0x30, 0, 3600, 0, 0},   // controlType
    {1, 0xE001, 0x0003, 0x20, 0, 3600, 1, 0},   // valueOpening
    {1, 0xE001, 0x0004, 0x10, 0, 3600, 0, 0},   // scaleSwitch
    {1, 0xE001, 0x0005, 0x10, 0, 3600, 0, 0},   // boostSwitch
    {1, 0xE001, 0x0006, 0x20, 0, 3600, 1, 0},   // boostCountdown
    {1, 0xE001, 0x0007, 0x30, 0, 3600, 0, 0},   // workMode
};

}  // namespace

extern const PreparedDefinition kDef_T000R_ZG{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T000R_ZG", .vendor="HzcElectric",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::hzc_electric
