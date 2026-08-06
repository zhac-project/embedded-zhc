// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Moes ZM6LT1 1-phase energy meter with CT sensor clamp
// (z2m v26.92.0 parity, added v26.78.0).
// z2m-source: moes.ts #ZM6LT1  (fingerprint TS0601 / _TZE284_2fnssffc)
//
// One of the three devices the family-modelID coverage bug hid.
//
// PARTIAL PORT -- voltage / current / power are DEFERRED.
//   Upstream reads them from DP6 via `tuya.valueConverter.phaseVariant5`, a
//   converter ADDED to lib/tuya.ts in this very window. It is not a scalar
//   datapoint: one DP carries a packed buffer decoded as
//     voltage = ((buf[2]<<8)|buf[3])/10
//     current = ((buf[5]<<8)|buf[6])/1000
//     power   =  (buf[8]<<8)|buf[9]
//   and its triple even has a null key, because it publishes three values at
//   once. A TuyaDpMapEntry maps one DP to one key, and embedded-zhc has no
//   phase-variant decoder at all yet (no phaseVariant1..5). Adding one unlocks
//   this device and every other Tuya meter that uses the family, so it is
//   worth doing as its own change rather than faked here.
//
// Covered: both energy totals, active energy, AC frequency, fault, online
// state, countdown and the two control DPs. Divisors are z2m divideBy100 on
// the energy/frequency DPs and raw elsewhere.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::moes {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kOnlineState[] = { {0, "offline"}, {1, "online"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::numeric(2, "reverse_energy", 100),
        // DP6 voltage/current/power DEFERRED -- phaseVariant5, see file header.
        ::zhc::tuya::dp::numeric(10, "fault", 1),
        ::zhc::tuya::dp::numeric(17, "alarm_set_2", 1),
        ::zhc::tuya::dp::binary(20, "clear_event"),
        ::zhc::tuya::dp::enum_lookup(44, "online_state", kOnlineState, 2),
        ::zhc::tuya::dp::numeric(49, "ac_frequency", 100),
        ::zhc::tuya::dp::numeric(51, "active_energy", 100),
        ::zhc::tuya::dp::numeric(101, "countdown_1", 1),
        ::zhc::tuya::dp::binary(104, "device_restart"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kOnlineStateOpts[] = { "offline", "online" };

constexpr Expose kExposes[] = {
    {"energy",         ExposeType::Numeric, Access::State,    "kWh", "Total forward active energy", nullptr,           0},
    {"reverse_energy", ExposeType::Numeric, Access::State,    "kWh", "Total reverse active energy", nullptr,           0},
    {"active_energy",  ExposeType::Numeric, Access::State,    "kWh", "Total active energy",         nullptr,           0},
    {"ac_frequency",   ExposeType::Numeric, Access::State,    "Hz",  "AC frequency",                nullptr,           0},
    {"fault",          ExposeType::Numeric, Access::State,    nullptr, "Fault status",              nullptr,           0},
    {"online_state",   ExposeType::Enum,    Access::State,    nullptr, "Online state",              kOnlineStateOpts,  2},
    {"alarm_set_2",    ExposeType::Numeric, Access::StateSet, nullptr, "Alarm setting",             nullptr,           0},
    {"clear_event",    ExposeType::Binary,  Access::StateSet, nullptr, "Clear event",               nullptr,           0},
    {"countdown_1",    ExposeType::Numeric, Access::StateSet, "s",     "Countdown timer",           nullptr,           0},
    {"device_restart", ExposeType::Binary,  Access::Set,      nullptr, "Device restart",            nullptr,           0},
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE284_2fnssffc" };
}  // namespace

extern const PreparedDefinition kDef_ZM6LT1{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="ZM6LT1", .vendor="Moes",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
};

}  // namespace zhc::devices::moes
