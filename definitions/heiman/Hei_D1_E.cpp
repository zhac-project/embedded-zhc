// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman D1-E smart door sensor (zigbeeModel D1-E), new in z2m
// v26.126.0..v26.133.0. m.battery + m.iasZoneAlarm contact (alarm_1, battery_low,
// tamper) like its D1-EFA sibling, plus heimanClusterSpecial (0xFC90, mfg 0x120B):
// heimanClusterIndicatorLight("alarm_indicator") on indicatorLightLevelControlOf1
// 0x1004 (u8, written with default response), and the reportedPackages 0x001B /
// rejoinedCount 0x001A / rebootedCount 0x0019 diagnostic counters (u16, read-only).
// The 0xFC90 rows only accept frames carrying the Heiman manufacturer code (the
// cluster has no name otherwise, so it would fail open).
// Not ported: identify, battery-percentage reporting.
// z2m-source: heiman.ts #D1-E
#include "definitions/_generic/_shared.hpp"
#include "definitions/heiman/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
using ::zhc::heiman::kHeimanMfgCode;

constexpr ::zhc::generic::ZclAttrRow kSpecialRows[] = {
    { 0x1004, "alarm_indicator", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x001B, "reported_packages" },
    { 0x001A, "rejoin_count" },
    { 0x0019, "reboot_count" },
};
constexpr ::zhc::generic::ZclAttrMap kSpecialMap{ kSpecialRows, sizeof(kSpecialRows)/sizeof(kSpecialRows[0]), kHeimanMfgCode };
constexpr FzConverter kFzSpecial = ::zhc::generic::zcl_attr_fz("heimanClusterSpecial", &kSpecialMap);
constexpr ::zhc::generic::ZclWriteSpec kAlarmIndicatorSpec{ "alarm_indicator", 0x1004, 0x20, kHeimanMfgCode, nullptr, 0,
                                                             ::zhc::generic::kZclWriteFlagDefaultResponse };
constexpr TzConverter kTzAlarmIndicator = ::zhc::generic::zcl_write_tz("heimanClusterSpecial", 0xFC90, &kAlarmIndicatorSpec);

const FzConverter* const kFz_D1_E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
    &kFzSpecial,
};
const TzConverter* const kTz_D1_E[] = {
    &kTzAlarmIndicator,
};
constexpr const char* kModels_D1_E[] = { "D1-E" };
constexpr Expose kExposes_D1_E[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"alarm_indicator", ExposeType::Binary, Access::StateSet, nullptr, "Enable/disable the indicator on product", nullptr, 0},
    {"reported_packages", ExposeType::Numeric, Access::State, nullptr,
     "For diagnostic purpose, how many zigbee packages the product reported in a day", nullptr, 0,
     ExposeCategory::Diagnostic, 0, 60000, 0},
    {"rejoin_count", ExposeType::Numeric, Access::State, nullptr,
     "For diagnostic purpose, how many times the product rejoined the zigbee network", nullptr, 0,
     ExposeCategory::Diagnostic, 0, 60000, 0},
    {"reboot_count", ExposeType::Numeric, Access::State, nullptr,
     "For diagnostic purpose, how many times the product rebooted", nullptr, 0,
     ExposeCategory::Diagnostic, 0, 60000, 0},
};
constexpr BindingSpec kBindings_D1_E[] = {
    {1, 0x0001},
    {1, 0xFC90},
};
}  // namespace

extern const PreparedDefinition kDef_D1_E{
    .zigbee_models=kModels_D1_E, .zigbee_models_count=sizeof(kModels_D1_E)/sizeof(kModels_D1_E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="D1-E", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_D1_E, .exposes_count=sizeof(kExposes_D1_E)/sizeof(kExposes_D1_E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D1_E, .from_zigbee_count=sizeof(kFz_D1_E)/sizeof(kFz_D1_E[0]),
    .to_zigbee=kTz_D1_E, .to_zigbee_count=sizeof(kTz_D1_E)/sizeof(kTz_D1_E[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D1_E, .bindings_count=sizeof(kBindings_D1_E)/sizeof(kBindings_D1_E[0]),
};

}  // namespace zhc::devices::heiman
