// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: hand-port. Heat Decor HD-T1000 floor thermostat.
// z2m-source: tuya.ts #HD-T1000 (fingerprint TS0601 / _TZE200_spyvfeti).
//
// Flat exposes per the project convention — z2m's e.climate() composite
// is not lowered into a single ExposeType::Climate. See
// [[feedback-climate-composite]] for the deferral rationale.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_HD_T1000_work_mode_dp2[] = {
    { 0, "manual" },
    { 1, "schedule" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_HD_T1000_running_state_dp3[] = {
    { 0, "heat" },
    { 1, "idle" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_HD_T1000[] = {
    // DP1 system_mode — z2m maps bool {heat:true, off:false}. Emitted as
    // Bool here; downstream maps to "heat"/"off" via shadow formatter.
    { 1, "system_mode", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "work_mode", ::zhc::TuyaDpType::Enum, 1,
        kEnum_HD_T1000_work_mode_dp2,
        sizeof(kEnum_HD_T1000_work_mode_dp2)/sizeof(kEnum_HD_T1000_work_mode_dp2[0]) },
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1,
        kEnum_HD_T1000_running_state_dp3,
        sizeof(kEnum_HD_T1000_running_state_dp3)/sizeof(kEnum_HD_T1000_running_state_dp3[0]) },
    { 10, "frost_protection",                ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8,  "window_detection",                ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "current_heating_setpoint",        ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 19, "max_air_temperature",             ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature",               ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 27, "local_temperature_calibration",   ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 40, "child_lock",                      ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "max_floor_temperature",          ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 102, "deadzone_temperature",           ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 104, "window_detection_time",          ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "window_detection_temperature",   ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "window_detection_recovery_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_HD_T1000{
    kEntries_HD_T1000,
    sizeof(kEntries_HD_T1000)/sizeof(kEntries_HD_T1000[0]),
};

constexpr FzConverter kFzDp_HD_T1000{
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
    .user_config       = &kMap_HD_T1000,
};
const FzConverter* const kFz_HD_T1000[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_HD_T1000,
};
constexpr TzConverter kTzDp_HD_T1000{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_HD_T1000,
};
const TzConverter* const kTz_HD_T1000[] = { &kTzDp_HD_T1000 };
constexpr const char* kModels_HD_T1000[] = { "TS0601" };
constexpr const char* kManus_HD_T1000[]  = { "_TZE200_spyvfeti" };

constexpr Expose kAutoExposes[] = {
    {"system_mode",                    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"work_mode",                      ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",                  ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"current_heating_setpoint",       ExposeType::Numeric, Access::StateSet, "°C",    nullptr, nullptr, 0},
    {"local_temperature",              ExposeType::Numeric, Access::State,    "°C",    nullptr, nullptr, 0},
    {"local_temperature_calibration",  ExposeType::Numeric, Access::StateSet, "°C",    nullptr, nullptr, 0},
    {"child_lock",                     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"frost_protection",               ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_detection",               ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"max_air_temperature",            ExposeType::Numeric, Access::StateSet, "°C",    nullptr, nullptr, 0},
    {"max_floor_temperature",          ExposeType::Numeric, Access::StateSet, "°C",    nullptr, nullptr, 0},
    {"deadzone_temperature",           ExposeType::Numeric, Access::StateSet, "°C",    nullptr, nullptr, 0},
    {"window_detection_time",          ExposeType::Numeric, Access::StateSet, "min",   nullptr, nullptr, 0},
    {"window_detection_temperature",   ExposeType::Numeric, Access::StateSet, "°C",    nullptr, nullptr, 0},
    {"window_detection_recovery_time", ExposeType::Numeric, Access::StateSet, "min",   nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
}  // namespace

extern const PreparedDefinition kDef_HD_T1000{
    .zigbee_models=kModels_HD_T1000,
    .zigbee_models_count=sizeof(kModels_HD_T1000)/sizeof(kModels_HD_T1000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_HD_T1000,
    .manufacturer_names_count=sizeof(kManus_HD_T1000)/sizeof(kManus_HD_T1000[0]),
    .model="HD-T1000",.vendor="Heat Decor",
    .meta=nullptr,
    .exposes=kAutoExposes,
    .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz_HD_T1000,
    .from_zigbee_count=sizeof(kFz_HD_T1000)/sizeof(kFz_HD_T1000[0]),
    .to_zigbee=kTz_HD_T1000,
    .to_zigbee_count=sizeof(kTz_HD_T1000)/sizeof(kTz_HD_T1000[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
    .bindings=kAutoBindings,
    .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::tuya
