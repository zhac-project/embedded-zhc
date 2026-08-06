// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Semicom touch panel — 2 switches + 2 shutters, _TZE200_rgeapp2c
// (z2m v26.92.0 parity, added v26.86.0).
// z2m-source: tuya.ts #_TZE200_rgeapp2c
//
// Divisors: `coverPosition` is the identity map unless the user sets the
// `invert_cover` option (lib/tuya.ts:1600), so position rides divisor 1. Use
// dp::position_inv only for devices upstream marks coverPositionInverted --
// this one is not.
//
// KNOWN GAP: z2m's coverPosition ALSO publishes a derived open/close state
// (`publish({state: position === 0 ? "CLOSE" : "OPEN"})`) as a side effect of
// decoding position. A TuyaDpMapEntry maps one DP to one key and cannot emit
// a second derived key, so that convenience state is not reproduced here. The
// real per-cover state is still covered by DP1/DP4 (state_c1 / state_c2),
// which is the value the device itself reports.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kOnOff[]      = { {0, "OFF"}, {1, "ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCoverState[] = { {0, "OPEN"}, {1, "STOP"}, {2, "CLOSE"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(101, "state_s1", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(102, "state_s2", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(1, "state_c1", kCoverState, 3),
        ::zhc::tuya::dp::numeric(2, "position_c1", 1),
        ::zhc::tuya::dp::enum_lookup(4, "state_c2", kCoverState, 3),
        ::zhc::tuya::dp::numeric(5, "position_c2", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kOnOffOpts[]      = { "OFF", "ON" };
constexpr const char* kCoverStateOpts[] = { "OPEN", "STOP", "CLOSE" };

constexpr Expose kExp[] = {
    { "state_s1",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 1 state",   kOnOffOpts,      2 },
    { "state_s2",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Switch 2 state",   kOnOffOpts,      2 },
    { "state_c1",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover 1 state",    kCoverStateOpts, 3 },
    { "position_c1", ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover 1 position", nullptr,         0 },
    { "state_c2",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover 2 state",    kCoverStateOpts, 3 },
    { "position_c2", ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover 2 position", nullptr,         0 },
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_rgeapp2c" };
}  // namespace

extern const PreparedDefinition kDef_TZE200_rgeapp2c{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,
    .model="_TZE200_rgeapp2c",.vendor="Semicom",
    .meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
