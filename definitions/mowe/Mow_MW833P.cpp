// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Mowe MW833P smart presence sensor, 24 GHz mmWave radar
// (`_TZE200_ops9sidw`). New vendor upstream in z2m v26.103.0 (#13061).
//
// Presence rides `trueFalseEnum1` upstream — an ENUM datapoint where 1 is
// true; the Bool decoder accepts any truthy 1-byte payload so `dp::binary`
// covers it. `sensitivity` is a plain 1..3 value, `body_motion` a 0..100
// amplitude pushed every 5 s (0 = nobody).
//
// NOT wired — four diagnostic TEXT datapoints (dp104 radar_detection_data,
// dp109 hardware_version, dp110 soft_version, dp111 radar_id): the datapoint
// decoder has no string type. Not exposed either.
//
// z2m-source: mowe.ts #MW833P.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::mowe {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kMotion[]    = { {0, "none"}, {1, "peaceful"}, {2, "motion"} };
constexpr ::zhc::tuya::TuyaEnumEntry kScene[]     = { {0, "default"}, {1, "area"}, {2, "toilet"}, {3, "bedroom"},
                                                      {4, "parlour"}, {5, "office"}, {6, "hotel"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDirection[] = { {0, "none"}, {1, "close_to"}, {2, "far_away"} };
constexpr ::zhc::tuya::TuyaEnumEntry kNobody[]    = { {0, "none"}, {1, "10s"}, {2, "30s"}, {3, "1min"}, {4, "2min"},
                                                      {5, "5min"}, {6, "10min"}, {7, "30min"}, {8, "1hour"} };

constexpr const char* kMotionOpts[]    = { "none", "peaceful", "motion" };
constexpr const char* kSceneOpts[]     = { "default", "area", "toilet", "bedroom", "parlour", "office", "hotel" };
constexpr const char* kDirectionOpts[] = { "none", "close_to", "far_away" };
constexpr const char* kNobodyOpts[]    = { "none", "10s", "30s", "1min", "2min", "5min", "10min", "30min", "1hour" };

struct cfg_mw833p {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1,   "presence"),
        ::zhc::tuya::dp::numeric(2,  "sensitivity", 1),
        ::zhc::tuya::dp::binary(101, "radar_reset_flag"),
        ::zhc::tuya::dp::enum_lookup(102, "human_motion_state", kMotion, 3),
        ::zhc::tuya::dp::numeric(103, "illuminance", 1),
        ::zhc::tuya::dp::binary(107, "check_end_flag"),
        ::zhc::tuya::dp::binary(108, "radar_self_check"),
        ::zhc::tuya::dp::enum_lookup(112, "scene", kScene, 7),
        ::zhc::tuya::dp::enum_lookup(114, "move_direction", kDirection, 3),
        ::zhc::tuya::dp::numeric(115, "body_motion", 1),
        ::zhc::tuya::dp::enum_lookup(131, "nobody_time", kNobody, 9),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_mw833p>;

constexpr Expose kExposes[] = {
    { "presence",           ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "human_motion_state", ExposeType::Enum,    ::zhc::Access::State, nullptr,
      "Motion state reported by the radar: nobody, present but still, or moving", kMotionOpts, 3 },
    { "illuminance",        ExposeType::Numeric, ::zhc::Access::State, "lx", nullptr, nullptr, 0 },
    { "body_motion",        ExposeType::Numeric, ::zhc::Access::State, nullptr,
      "Body-motion amplitude, emitted every 5 seconds. 0 means nobody detected", nullptr, 0, ExposeCategory::State, 0, 100, 1 },
    { "move_direction",     ExposeType::Enum,    ::zhc::Access::State, nullptr,
      "Direction of the detected movement relative to the sensor", kDirectionOpts, 3 },
    { "sensitivity",        ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,
      "Radar sensitivity, 1 (least sensitive) to 3 (most sensitive)", nullptr, 0, ExposeCategory::Config, 1, 3, 1 },
    { "scene",              ExposeType::Enum,    ::zhc::Access::StateSet, nullptr,
      "Detection profile tuned for the size and use of the room", kSceneOpts, 7, ExposeCategory::Config },
    { "nobody_time",        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr,
      "How long presence is held after the last detection before clearing", kNobodyOpts, 9, ExposeCategory::Config },
    { "radar_self_check",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,
      "Start the radar self-check routine", nullptr, 0, ExposeCategory::Config },
    { "check_end_flag",     ExposeType::Binary,  ::zhc::Access::State, nullptr,
      "Radar self-check has finished", nullptr, 0, ExposeCategory::Diagnostic },
    { "radar_reset_flag",   ExposeType::Binary,  ::zhc::Access::State, nullptr,
      "Radar reset marker, pushed by the device", nullptr, 0, ExposeCategory::Diagnostic },
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE200_ops9sidw" };

}  // namespace

extern const PreparedDefinition kDef_MW833P{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="MW833P", .vendor="Mowe",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::mowe
