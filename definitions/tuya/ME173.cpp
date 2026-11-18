// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Ecobuy Well ME173 (`_TZE200_nhxzhga5`) thermostatic radiator valve —
// new upstream in z2m v26.133.0.
//
// DP2 is one mode enum that z2m fans out to `preset` and `system_mode`
// (thermostatSystemModeAndPresetMap): both rows decode it; a system_mode write
// hits the first table match (auto=0, heat=1, off=2), as upstream's toMap.
//
// DP47 calibration arrives as int8 or int32 (z2m: v > 127 -> v - 256); the
// write sends the plain s32 like z2m's `to: v => v`, so it is a local decoder
// and not kTuyaDpFlagSigned8 (that one encodes -3 as 253).
//
// Not ported: DP 28-34 weekly schedule (thermostatScheduleDayMultiDPWithDayNumber
// codec gap).
//
// z2m-source: tuya.ts #ME173.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

bool expand_me173_calibration(const ::zhc::tuya::TuyaDpMapEntry& e, const Value& raw,
                              RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (raw.type != ValueType::Int) return false;
    std::int64_t v = raw.i;              // s32 already sign-extended
    if (v > 127) v -= 256;               // int8 form
    Value o{}; o.type = ValueType::Int; o.i = v;
    out.put(e.out_key, o);
    return true;
}

constexpr ::zhc::tuya::TuyaEnumEntry kMe173Preset[] = {
    {0, "schedule"}, {1, "manual"}, {2, "off"}, {3, "eco"}, {4, "comfort"}, {5, "boost"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kMe173SystemMode[] = {
    {0, "auto"}, {1, "heat"}, {2, "off"}, {3, "heat"}, {4, "heat"}, {5, "heat"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kMe173RunningState[] = { {0, "heat"}, {1, "idle"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMe173WindowOpen[]   = { {0, "OPEN"}, {1, "CLOSE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMe173Display[]      = { {0, "left"}, {1, "right"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMe173OnOff[]        = { {0, "OFF"}, {1, "ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMe173Lock[]         = { {0, "UNLOCK"}, {1, "LOCK"} };

constexpr std::uint16_t kMe173BoolEnum = ::zhc::tuya::kTuyaDpFlagBoolEnum;

struct cfg_me173 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(2, "preset", kMe173Preset, 6),
        ::zhc::tuya::dp::enum_lookup(2, "system_mode", kMe173SystemMode, 6),
        ::zhc::tuya::dp::enum_lookup(3, "running_state", kMe173RunningState, 2),
        ::zhc::tuya::dp::numeric(4, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(5, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(6, "battery", 1),
        { 7,  "child_lock",       ::zhc::TuyaDpType::Bool, 1, kMe173Lock,  2, kMe173BoolEnum },
        { 14, "window_detection", ::zhc::TuyaDpType::Bool, 1, kMe173OnOff, 2, kMe173BoolEnum },
        ::zhc::tuya::dp::enum_lookup(15, "window_open", kMe173WindowOpen, 2),
        ::zhc::tuya::dp::numeric(35, "error", 1),
        { 36, "frost_protection", ::zhc::TuyaDpType::Bool, 1, kMe173OnOff, 2, kMe173BoolEnum },
        { 39, "scale_protection", ::zhc::TuyaDpType::Bool, 1, kMe173OnOff, 2, kMe173BoolEnum },
        { 47, "local_temperature_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, 0.0f,
          &expand_me173_calibration, nullptr },
        ::zhc::tuya::dp::numeric(101, "comfort_temperature", 10),
        ::zhc::tuya::dp::numeric(103, "boost_time", 1),
        ::zhc::tuya::dp::numeric(104, "boost_timeset_countdown", 1),
        ::zhc::tuya::dp::numeric(105, "eco_temperature", 10),
        ::zhc::tuya::dp::enum_lookup(107, "display_orientation", kMe173Display, 2),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXMe173 = ::zhc::tuya::factory::TuyaRw<cfg_me173>;

constexpr const char* kMe173SystemModeOpts[]   = { "off", "heat", "auto" };
constexpr const char* kMe173PresetOpts[]       = { "schedule", "manual", "off", "eco", "comfort", "boost" };
constexpr const char* kMe173RunningStateOpts[] = { "idle", "heat" };
constexpr const char* kMe173DisplayOpts[]      = { "left", "right" };

// Flat climate exposes (project rule). 0.5 °C steps are not expressible in
// the integer value_step, left unspecified.
constexpr Expose kExposes_Me173[] = {
    { "child_lock", ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Enables/disables physical input on the device", nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",
      "Remaining battery in %", nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0 },
    { "current_heating_setpoint", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",
      "Temperature setpoint", nullptr, 0, ExposeCategory::State, 5, 35, 0 },
    { "local_temperature", ExposeType::Numeric, ::zhc::Access::State, "°C",
      "Current temperature measured on the device", nullptr, 0 },
    { "system_mode", ExposeType::Enum, ::zhc::Access::StateSet, nullptr,
      "Mode of this device", kMe173SystemModeOpts, 3 },
    { "preset", ExposeType::Enum, ::zhc::Access::StateSet, nullptr,
      "Mode of this device (similar to system_mode)", kMe173PresetOpts, 6 },
    { "running_state", ExposeType::Enum, ::zhc::Access::State, nullptr,
      "The current running state", kMe173RunningStateOpts, 2 },
    { "local_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",
      "Offset to add/subtract to the local temperature", nullptr, 0, ExposeCategory::State, -9, 9, 1 },
    { "eco_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",
      "Eco temperature", nullptr, 0, ExposeCategory::State, 10, 20, 0 },
    { "comfort_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",
      "Comfort temperature", nullptr, 0, ExposeCategory::State, 10, 25, 0 },
    { "window_detection", ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Enable open window detection", nullptr, 0 },
    { "window_open", ExposeType::Binary, ::zhc::Access::State, nullptr,
      "Window status", nullptr, 0 },
    { "scale_protection", ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Opens the valve fully for 30 seconds every two weeks to prevent scaling (displays \"Ad\")", nullptr, 0 },
    { "frost_protection", ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Opens the valve below 5 °C and closes it again at 8 °C, also when the valve is off", nullptr, 0 },
    { "error", ExposeType::Numeric, ::zhc::Access::State, nullptr,
      "If NTC is damaged, \"Er\" will be on the TRV display.", nullptr, 0 },
    { "display_orientation", ExposeType::Enum, ::zhc::Access::StateSet, nullptr,
      "Display orientation", kMe173DisplayOpts, 2 },
    { "boost_time", ExposeType::Numeric, ::zhc::Access::StateSet, "min",
      "Duration of the boost preset, which heats at 35 °C", nullptr, 0, ExposeCategory::State, 0, 180, 0 },
    { "boost_timeset_countdown", ExposeType::Numeric, ::zhc::Access::State, "min",
      "Remaining boost time", nullptr, 0 },
};

constexpr const char* kM_Me173[] = { "TS0601" };
constexpr const char* kN_Me173[] = { "_TZE200_nhxzhga5" };

}  // namespace

extern const PreparedDefinition kDef_ME173{
    .zigbee_models=kM_Me173, .zigbee_models_count=sizeof(kM_Me173)/sizeof(kM_Me173[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_Me173, .manufacturer_names_count=sizeof(kN_Me173)/sizeof(kN_Me173[0]),
    .model="ME173", .vendor="Ecobuy Well",
    .meta=nullptr,
    .exposes=kExposes_Me173, .exposes_count=sizeof(kExposes_Me173)/sizeof(kExposes_Me173[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXMe173::fz_list, .from_zigbee_count=FXMe173::fz_count,
    .to_zigbee=FXMe173::tz_list, .to_zigbee_count=FXMe173::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::tuya
