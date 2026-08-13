// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Semicom touch panel — 3 shutters (z2m v26.95.0 parity, added v26.95.0).
// z2m-source: tuya.ts #_TZE204_7lb6j8wg  (TS0601 / _TZE204_7lb6j8wg)
//
// Sibling of _TZE200_rgeapp2c (2 switches + 2 shutters). coverPosition is the
// identity map unless the user sets invert_cover, so position rides divisor 1.
// Same known gap as that device: z2m's coverPosition also publishes a derived
// OPEN/CLOSE state as a side effect, which a one-DP-to-one-key map cannot do;
// the real per-cover state is carried by DP1/DP4/DP101.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kCoverState3[] = { {0, "OPEN"}, {1, "STOP"}, {2, "CLOSE"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state_c1", kCoverState3, 3),
        ::zhc::tuya::dp::numeric(2, "position_c1", 1),
        ::zhc::tuya::dp::enum_lookup(4, "state_c2", kCoverState3, 3),
        ::zhc::tuya::dp::numeric(5, "position_c2", 1),
        ::zhc::tuya::dp::enum_lookup(101, "state_c3", kCoverState3, 3),
        ::zhc::tuya::dp::numeric(102, "position_c3", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kCoverOpts[] = { "OPEN", "STOP", "CLOSE" };
constexpr Expose kExp[] = {
    {"state_c1",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover 1 state",    kCoverOpts, 3},
    {"position_c1", ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover 1 position", nullptr,    0},
    {"state_c2",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover 2 state",    kCoverOpts, 3},
    {"position_c2", ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover 2 position", nullptr,    0},
    {"state_c3",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Cover 3 state",    kCoverOpts, 3},
    {"position_c3", ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Cover 3 position", nullptr,    0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_7lb6j8wg" };
}  // namespace
extern const PreparedDefinition kDef_TZE204_7lb6j8wg{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="_TZE204_7lb6j8wg",.vendor="Semicom",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
