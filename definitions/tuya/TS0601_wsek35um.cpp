// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_wsek35um radiator thermostat (z2m v26.112.0, #13227).
// Upstream's extra converter publishes current_heating_setpoint = 5 when DP2 (mode)
// reports 0 (Off) — reproduced with a second, decode-only row on DP2 placed after
// DP4 so a setpoint write still goes to DP4.
// z2m-source: tuya.ts #TS0601_wsek35um.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kMode2[] = { {0,"Off"}, {1,"Cold"}, {2,"Night"}, {3,"Day"}, {4,"Scheduled"} };
// z2m ts0601Wsek35umModeSetpointConverter: mode Off (0) reads as a 5 °C setpoint.
bool mode_off_setpoint(const ::zhc::tuya::TuyaDpMapEntry& e, const Value& raw, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const bool off = (raw.type == ValueType::Int && raw.i == 0) || (raw.type == ValueType::Uint && raw.u == 0);
    if (!off) return false;
    Value v{}; v.type = ValueType::Float; v.f = 5.0f;
    out.put(e.out_key, v);
    return true;
}
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 2, "mode", ::zhc::TuyaDpType::Numeric, 1, kMode2, 5, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(4, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(5, "local_temperature", 10),
        ::zhc::tuya::dp::binary(7, "children_lock"),
        { 2, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, 0.0f, &mode_off_setpoint, nullptr },
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts3[] = { "Off", "Cold", "Night", "Day", "Scheduled" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", "Current temperature measured on the device", nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", "Temperature setpoint", nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"children_lock", ExposeType::Binary, Access::State, nullptr, "Children lock status", nullptr, 0},
    {"mode", ExposeType::Enum, Access::State, nullptr, "Current mode", kOpts3, 5},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_wsek35um" };
}  // namespace

extern const PreparedDefinition kDef_TS0601_wsek35um{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_wsek35um",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::tuya
