// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Alecto SMART-HEAT10 — Tuya-MCU (0xEF00 manuSpecificTuya) radiator
// valve / TRV thermostat (TS0601 / _TZE200_8daqwrsj + _TYST11_8daqwrsj).
//
// Bug fixed: the auto extract ported this as the generic kFzThermostat /
// kTzThermostat (standard hvacThermostat 0x0201 cluster) bound to EP1 0x0201.
// The device speaks NONE of that — z2m decodes it through the legacy Tuya TRV
// converter legacy.fz.tuya_thermostat (a 0xEF00 siterwell-style DP stream),
// so the 0x0201 wiring was a dead misroute and child_lock / window_detection /
// battery were dropped entirely. Re-wired to fz_tuya_datapoints +
// tz_tuya_datapoints with the device's DP map:
//   DP 2  current_heating_setpoint  numeric /10  (z2m value/10)
//   DP 3  local_temperature         numeric /10  (z2m value/10)
//   DP 4  system_mode               enum {0: off, 1: auto, 2: heat}
//                                    (z2m mode DP -> preset -> presetToSystemMode
//                                     = thermostatSystemModes4 keyed by the DP value)
//   DP 7  child_lock                bool -> "UNLOCK"/"LOCK" (z2m value?LOCK:UNLOCK)
//   DP 18 window_detection          bool -> "OFF"/"ON" (siterwellWindowDetection)
//   DP 21 battery                   numeric % (raw, no scale)
//
// Project rule: thermostats ship FLAT exposes (no ExposeType::Climate). The
// generic kFzThermostat only decodes 0x0000/0x0012/0x001C anyway — useless for
// this DP-stream device.
//
// SUSPECT (wrong scale on setpoint/local_temperature) confirmed: z2m applies
// /10 to both (NOT /1), so the DP map carries divisor 10 -> emit Float.
//
// z2m-source: alecto.ts #SMART-HEAT10 + lib/legacy.ts fz.tuya_thermostat
//             (siterwellWindowDetection / childLock / heatingSetpoint /
//             localTemp / mode / battery DPs) + thermostatSystemModes4.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::alecto {
namespace {

// z2m: child_lock = value ? "LOCK" : "UNLOCK" (bool DP fanned to string).
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_HEAT10_child_lock_dp7[] = {
    { 0, "UNLOCK" },
    { 1, "LOCK" },
};
// z2m: window_detection = value ? "ON" : "OFF" (siterwellWindowDetection bool).
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_HEAT10_window_detection_dp18[] = {
    { 0, "OFF" },
    { 1, "ON" },
};
// z2m: system_mode via preset -> presetToSystemMode (thermostatSystemModes4),
// both keyed by the same DP-4 value: 0=off, 1=auto, 2=heat.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_HEAT10_system_mode_dp4[] = {
    { 0, "off" },
    { 1, "auto" },
    { 2, "heat" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_HEAT10[] = {
    { 2,  "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 3,  "local_temperature",        ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 4,  "system_mode",              ::zhc::TuyaDpType::Enum,     1, kEnum_HEAT10_system_mode_dp4,      sizeof(kEnum_HEAT10_system_mode_dp4)/sizeof(kEnum_HEAT10_system_mode_dp4[0]),           0 },
    { 7,  "child_lock",               ::zhc::TuyaDpType::Bool,     1, kEnum_HEAT10_child_lock_dp7,       sizeof(kEnum_HEAT10_child_lock_dp7)/sizeof(kEnum_HEAT10_child_lock_dp7[0]),             ::zhc::tuya::kTuyaDpFlagBoolEnum },
    { 18, "window_detection",         ::zhc::TuyaDpType::Bool,     1, kEnum_HEAT10_window_detection_dp18, sizeof(kEnum_HEAT10_window_detection_dp18)/sizeof(kEnum_HEAT10_window_detection_dp18[0]), ::zhc::tuya::kTuyaDpFlagBoolEnum },
    { 21, "battery",                  ::zhc::TuyaDpType::Numeric,  1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_HEAT10{
    kEntries_HEAT10, sizeof(kEntries_HEAT10)/sizeof(kEntries_HEAT10[0]) };

constexpr FzConverter kFzDp_HEAT10{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kMap_HEAT10,
};
const FzConverter* const kFz_SMART_HEAT10[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_HEAT10,
};
constexpr TzConverter kTzDp_HEAT10{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_HEAT10,
};
const TzConverter* const kTz_SMART_HEAT10[] = { &kTzDp_HEAT10 };

constexpr const char* kModels_SMART_HEAT10[] = { "daqwrsj", "TS0601" };
constexpr const char* kManus_SMART_HEAT10[] = { "_TYST11_8daqwrsj", "_TZE200_8daqwrsj" };

// z2m exposes (#SMART-HEAT10) — FLAT (no ExposeType::Climate per project rule).
// current_heating_setpoint + system_mode are STATE_SET; child_lock /
// window_detection are writable; local_temperature + battery are read-only.
constexpr Expose kExp_SMART_HEAT10[] = {
    { "local_temperature",        ExposeType::Numeric, ::zhc::Access::State,    "°C", nullptr, nullptr, 0 },
    { "current_heating_setpoint", ExposeType::Numeric, ::zhc::Access::StateSet, "°C", nullptr, nullptr, 0 },
    { "system_mode",              ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "child_lock",               ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "window_detection",         ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery",                  ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_SMART_HEAT10[] = {
    { 1, 0xEF00 },
};
}  // namespace

extern const PreparedDefinition kDef_SMART_HEAT10{
    .zigbee_models=kModels_SMART_HEAT10, .zigbee_models_count=sizeof(kModels_SMART_HEAT10)/sizeof(kModels_SMART_HEAT10[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SMART_HEAT10, .manufacturer_names_count=sizeof(kManus_SMART_HEAT10)/sizeof(kManus_SMART_HEAT10[0]),
    .model="SMART-HEAT10", .vendor="Alecto",
    .meta=nullptr, .exposes=kExp_SMART_HEAT10, .exposes_count=sizeof(kExp_SMART_HEAT10)/sizeof(kExp_SMART_HEAT10[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SMART_HEAT10, .from_zigbee_count=sizeof(kFz_SMART_HEAT10)/sizeof(kFz_SMART_HEAT10[0]),
    .to_zigbee=kTz_SMART_HEAT10, .to_zigbee_count=sizeof(kTz_SMART_HEAT10)/sizeof(kTz_SMART_HEAT10[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_SMART_HEAT10, .bindings_count=sizeof(kBind_SMART_HEAT10)/sizeof(kBind_SMART_HEAT10[0]),
};

}  // namespace zhc::devices::alecto
