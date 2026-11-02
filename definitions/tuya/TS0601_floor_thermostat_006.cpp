// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_floor_thermostat_006, electric floor thermostat with
// internal and external sensors, TS0601 _TZE204_6a4vxfnv / _TZE204_0hhou4yj
// (ELECTSMART EST-120Z / EST-D100Z). z2m v26.125.0 moved 6a4vxfnv here from
// TS0601_floor_thermostat; the generated copy of that old def decoded this
// hardware wrongly (calibration /100, child_lock on the eco DP, probe
// temperature as local_temperature).
// DPs: 1 system_mode (Bool heat/off), 2 preset tv02Preset (auto, manual,
// holiday), 16 setpoint /10, 24 internal sensor /10, 27 calibration raw,
// 36 running_state, 39 child_lock, 40 eco_mode, 43 sensor IN/AL/OU, 102
// external sensor /10, 103 deadzone /10. local_temperature follows the
// active sensor: DP24 feeds it unless sensor = OU, DP102 only when it is OU;
// the sensor is cached per device in DeviceRuntimeState::scratch[0] (before
// any DP43 report it is taken as IN, as upstream does with unset state).
// Not ported: republishing local_temperature from the cached reading the
// moment DP43 changes (the next sensor report updates it) and the weekly
// schedule DPs 101/105-110 (thermostatScheduleDayMultiDP has no codec here).
// Time base 1970.
// z2m-source: tuya.ts #TS0601_floor_thermostat_006.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr std::size_t  kSensorSlot = 0;   // DeviceRuntimeState::scratch index
constexpr std::uint8_t kSensorOu   = 2;

bool sensor_is_ou(RuntimeContext& ctx) {
    const auto* st = ctx.device_state();
    return st && st->scratch[kSensorSlot] == kSensorOu;
}

void put_float(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key, float f) {
    Value v{}; v.type = ValueType::Float; v.f = f;
    out.put(key, v);
}

// DP24 internal sensor: internal_temperature, and local_temperature unless
// the external sensor (OU) is in control.
bool expand_internal(const ::zhc::tuya::TuyaDpMapEntry&, const Value& raw, RuntimeContext& ctx,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (raw.type != ValueType::Int) return false;
    const float t = static_cast<float>(raw.i) / 10.0f;
    put_float(out, "internal_temperature", t);
    if (!sensor_is_ou(ctx)) put_float(out, "local_temperature", t);
    return true;
}

// DP102 external sensor: external_temperature_input, and local_temperature
// when the external sensor is in control.
bool expand_external(const ::zhc::tuya::TuyaDpMapEntry&, const Value& raw, RuntimeContext& ctx,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (raw.type != ValueType::Int) return false;
    const float t = static_cast<float>(raw.i) / 10.0f;
    put_float(out, "external_temperature_input", t);
    if (sensor_is_ou(ctx)) put_float(out, "local_temperature", t);
    return true;
}

constexpr ::zhc::tuya::TuyaEnumEntry kSensor[] = { {0, "IN"}, {1, "AL"}, {2, "OU"} };

// DP43 sensor: publishes the label and caches the wire value.
bool expand_sensor(const ::zhc::tuya::TuyaDpMapEntry& e, const Value& raw, RuntimeContext& ctx,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (raw.type != ValueType::Uint || raw.u > kSensorOu) return false;
    if (auto* st = ctx.device_state()) st->scratch[kSensorSlot] = static_cast<std::uint8_t>(raw.u);
    Value v{}; v.type = ValueType::StringRef; v.str = kSensor[raw.u].label;
    out.put(e.out_key, v);
    return true;
}

constexpr ::zhc::tuya::TuyaEnumEntry kSystemMode[] = { {1, "heat"}, {0, "off"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPreset[] = { {0, "auto"}, {1, "manual"}, {2, "holiday"}, {3, "holiday"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRunning[] = { {0, "heat"}, {1, "idle"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 1, "system_mode", ::zhc::TuyaDpType::Bool, 1, kSystemMode, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::enum_lookup(2, "preset", kPreset, 4),
        ::zhc::tuya::dp::numeric(16, "current_heating_setpoint", 10),
        { 24, "internal_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0, 0.0f, &expand_internal, nullptr },
        ::zhc::tuya::dp::numeric(27, "local_temperature_calibration", 1),
        ::zhc::tuya::dp::enum_lookup(36, "running_state", kRunning, 2),
        ::zhc::tuya::dp::binary(39, "child_lock"),
        ::zhc::tuya::dp::binary(40, "eco_mode"),
        { 43, "sensor", ::zhc::TuyaDpType::Enum, 1, kSensor, 3, 0, 0.0f, &expand_sensor, nullptr },
        { 102, "external_temperature_input", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0, 0.0f, &expand_external, nullptr },
        ::zhc::tuya::dp::numeric(103, "deadzone_temperature", 10),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kSystemModeOpts[] = { "off", "heat" };
constexpr const char* kPresetOpts[] = { "manual", "auto" };
constexpr const char* kRunningOpts[] = { "idle", "heat" };
constexpr const char* kSensorOpts[] = { "IN", "OU", "AL" };
constexpr Expose kExp[] = {
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModeOpts, 2},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPresetOpts, 2},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kRunningOpts, 2},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", "Temperature of the active control sensor", nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, -9, 9, 0},
    {"internal_temperature", ExposeType::Numeric, Access::State, "°C", "Temperature reported by the internal sensor", nullptr, 0},
    {"external_temperature_input", ExposeType::Numeric, Access::State, "°C", "Temperature reported by the external floor sensor", nullptr, 0},
    {"sensor", ExposeType::Enum, Access::StateSet, nullptr,
     "IN: internal sensor; OU: external sensor; AL: internal control with external floor limit", kSensorOpts, 3},
    {"eco_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"deadzone_temperature", ExposeType::Numeric, Access::StateSet, "°C", "Heating hysteresis", nullptr, 0, ExposeCategory::Config, 0, 5, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_6a4vxfnv", "_TZE204_0hhou4yj" };
constexpr WhiteLabel kWhiteLabels[] = { {"ELECTSMART", "EST-120Z"}, {"ELECTSMART", "EST-D100Z"} };
}  // namespace

extern const PreparedDefinition kDef_TS0601_floor_thermostat_006{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_floor_thermostat_006",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWhiteLabels,.white_labels_count=sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::tuya
