// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_hdml1aav).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_hdml1aav_water_warning_dp111[] = {
    { 0, "none" },
    { 1, "alarm" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_hdml1aav_soil_fertility_warning_dp116[] = {
    { 0, "none" },
    { 1, "low" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_hdml1aav[] = {
    { 3, "soil_moisture", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "humidity", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "illuminance", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "report_period", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "soil_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "humidity_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "illuminance_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 110, "soil_warning", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 111, "water_warning", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_hdml1aav_water_warning_dp111, sizeof(kEnum__TZE284_hdml1aav_water_warning_dp111)/sizeof(kEnum__TZE284_hdml1aav_water_warning_dp111[0]) },
    { 112, "soil_fertility", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 113, "soil_fertility_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 114, "soil_fertility_set_v0", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 115, "soil_fertility_set_v1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 116, "soil_fertility_warning", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_hdml1aav_soil_fertility_warning_dp116, sizeof(kEnum__TZE284_hdml1aav_soil_fertility_warning_dp116)/sizeof(kEnum__TZE284_hdml1aav_soil_fertility_warning_dp116[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_hdml1aav{ kEntries__TZE284_hdml1aav, 17 };
constexpr FzConverter kFzDp__TZE284_hdml1aav{
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
    .user_config       = &kMap__TZE284_hdml1aav,
};
const FzConverter* const kFz__TZE284_hdml1aav[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_hdml1aav,
};
constexpr TzConverter kTzDp__TZE284_hdml1aav{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_hdml1aav,
};
const TzConverter* const kTz__TZE284_hdml1aav[] = { &kTzDp__TZE284_hdml1aav };
constexpr const char* kM__TZE284_hdml1aav[] = { "TS0601" };
constexpr const char* kN__TZE284_hdml1aav[] = { "_TZE284_hdml1aav" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_hdml1aav_0[] = {"none", "alarm"};
constexpr const char* kAutoOpts__TZE284_hdml1aav_1[] = {"none", "low", "high"};
constexpr Expose kAutoExposes__TZE284_hdml1aav[] = {
    {"water_warning", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_hdml1aav_0, 2},
    {"soil_fertility_warning", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE284_hdml1aav_1, 3},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::State, 1, 100, 1},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"soil_fertility", ExposeType::Numeric, Access::State, "μS/cm", nullptr, nullptr, 0, ExposeCategory::State, 0, 5000, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"report_period", ExposeType::Numeric, Access::StateSet, "S", nullptr, nullptr, 0, ExposeCategory::State, 5, 1200, 5},
    {"soil_calibration", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, -30, 30, 1},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, -30, 30, 1},
    {"illuminance_calibration", ExposeType::Numeric, Access::StateSet, "lux", nullptr, nullptr, 0, ExposeCategory::State, -15, 1000, 1},
    {"temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -2, 2, 0},
    {"soil_warning", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"soil_fertility_calibration", ExposeType::Numeric, Access::StateSet, "μS/cm", nullptr, nullptr, 0, ExposeCategory::State, -1000, 1000, 5},
    {"soil_fertility_set_v0", ExposeType::Numeric, Access::StateSet, "μS/cm", nullptr, nullptr, 0, ExposeCategory::State, 0, 5000, 1},
    {"soil_fertility_set_v1", ExposeType::Numeric, Access::StateSet, "μS/cm", nullptr, nullptr, 0, ExposeCategory::State, 0, 5000, 1},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_hdml1aav{
    .zigbee_models=kM__TZE284_hdml1aav,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_hdml1aav,.manufacturer_names_count=1,
    .model="TS0601__TZE284_hdml1aav",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_hdml1aav,.exposes_count=sizeof(kAutoExposes__TZE284_hdml1aav)/sizeof(kAutoExposes__TZE284_hdml1aav[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_hdml1aav,
    .from_zigbee_count=sizeof(kFz__TZE284_hdml1aav)/sizeof(kFz__TZE284_hdml1aav[0]),
    .to_zigbee=kTz__TZE284_hdml1aav,
    .to_zigbee_count=sizeof(kTz__TZE284_hdml1aav)/sizeof(kTz__TZE284_hdml1aav[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
