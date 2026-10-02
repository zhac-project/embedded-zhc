// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Moes MG-BJQ002 plug-in siren with RGB night light (z2m v26.111.0, #13201).
// z2m-source: tuya.ts #MG-BJQ002.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kAlarm1[] = { {0,"sound"}, {1,"light"}, {2,"sound_light"}, {3,"off"} };
constexpr ::zhc::tuya::TuyaEnumEntry kVolume5[] = { {0,"low"}, {1,"medium"}, {2,"high"}, {3,"mute"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRingtone21[] = { {0,"ringtone 1"}, {1,"ringtone 2"}, {2,"ringtone 3"}, {3,"ringtone 4"}, {4,"ringtone 5"}, {5,"ringtone 6"}, {6,"ringtone 7"}, {7,"ringtone 8"}, {8,"ringtone 9"}, {9,"ringtone 10"}, {10,"ringtone 11"}, {11,"ringtone 12"}, {12,"ringtone 13"}, {13,"ringtone 14"}, {14,"ringtone 15"}, {15,"ringtone 16"}, {16,"ringtone 17"}, {17,"ringtone 18"}, {18,"ringtone 19"}, {19,"ringtone 20"}, {20,"ringtone 21"}, {21,"ringtone 22"}, {22,"ringtone 23"}, {23,"ringtone 24"}, {24,"ringtone 25"}, {25,"ringtone 26"}, {26,"ringtone 27"}, {27,"ringtone 28"}, {28,"ringtone 29"}, {29,"ringtone 30"}, {30,"ringtone 31"}, {31,"ringtone 32"} };
constexpr ::zhc::tuya::TuyaEnumEntry kLightMode23[] = { {0,"rainbow"}, {1,"red_flash"}, {2,"night_light"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "alarm", kAlarm1, 4),
        ::zhc::tuya::dp::enum_lookup(5, "volume", kVolume5, 4),
        ::zhc::tuya::dp::numeric(7, "duration", 1),
        ::zhc::tuya::dp::enum_lookup(21, "ringtone", kRingtone21, 32),
        ::zhc::tuya::dp::binary(22, "night_light"),
        ::zhc::tuya::dp::enum_lookup(23, "light_mode", kLightMode23, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts0[] = { "sound", "light", "sound_light", "off" };
constexpr const char* kOpts2[] = { "low", "medium", "high", "mute" };
constexpr const char* kOpts3[] = { "ringtone 1", "ringtone 2", "ringtone 3", "ringtone 4", "ringtone 5", "ringtone 6", "ringtone 7", "ringtone 8", "ringtone 9", "ringtone 10", "ringtone 11", "ringtone 12", "ringtone 13", "ringtone 14", "ringtone 15", "ringtone 16", "ringtone 17", "ringtone 18", "ringtone 19", "ringtone 20", "ringtone 21", "ringtone 22", "ringtone 23", "ringtone 24", "ringtone 25", "ringtone 26", "ringtone 27", "ringtone 28", "ringtone 29", "ringtone 30", "ringtone 31", "ringtone 32" };
constexpr const char* kOpts5[] = { "rainbow", "red_flash", "night_light" };
constexpr Expose kExp[] = {
    {"alarm", ExposeType::Enum, Access::StateSet, nullptr, "Trigger the alarm (sound, light or both) for the configured duration, or stop it with 'off'", kOpts0, 4},
    {"duration", ExposeType::Numeric, Access::StateSet, "s", "How long the alarm sounds for when triggered", nullptr, 0, ExposeCategory::State, 1, 1800, 1},
    {"volume", ExposeType::Enum, Access::StateSet, nullptr, "Alarm volume", kOpts2, 4},
    {"ringtone", ExposeType::Enum, Access::StateSet, nullptr, "Alarm melody", kOpts3, 32},
    {"night_light", ExposeType::Binary, Access::StateSet, nullptr, "Night light", nullptr, 0},
    {"light_mode", ExposeType::Enum, Access::StateSet, nullptr, "RGB light mode", kOpts5, 3},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE20C_tjz9ad5g" };
}  // namespace

extern const PreparedDefinition kDef_MG_BJQ002{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="MG-BJQ002",.vendor="Moes",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
