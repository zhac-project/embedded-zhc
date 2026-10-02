// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Immax NEO 07519L water leak sensor (z2m v26.109.0, #13149).
// DP1 reports 0 on leakage (trueFalse0); DP101 silent_mode is ON at enum 0.
// z2m-source: immax.ts #07519L.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::immax {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSilentMode101[] = { {0,"ON"}, {1,"OFF"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRingtone102[] = { {0,"tone_1"}, {1,"tone_2"}, {2,"tone_3"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "water_leak"),
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        ::zhc::tuya::dp::enum_lookup(101, "silent_mode", kSilentMode101, 2),
        ::zhc::tuya::dp::enum_lookup(102, "ringtone", kRingtone102, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts2[] = { "ON", "OFF" };
constexpr const char* kOpts3[] = { "tone_1", "tone_2", "tone_3" };
constexpr Expose kExp[] = {
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"silent_mode", ExposeType::Enum, Access::StateSet, nullptr, "Mute the buzzer on leak detection (does not mute ongoing alarm)", kOpts2, 2, ExposeCategory::Config},
    {"ringtone", ExposeType::Enum, Access::StateSet, nullptr, "Selected buzzer ringtone for the alarm", kOpts3, 3, ExposeCategory::Config},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_rhocfd6y" };
}  // namespace

extern const PreparedDefinition kDef_D07519L{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="07519L",.vendor="Immax",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::immax
