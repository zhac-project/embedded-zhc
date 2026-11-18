// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand WNRR15/WNRR20 — generated, hand-edited for z2m v26.133.0
// (R151); do not let the generator clobber it.
// Outlet with power consumption monitoring
//
// v26.133.0 adds the 064887 white label, power_on_behavior (genOnOff 0x4003),
// the Legrand LED modes (0xFC01 led_in_dark / led_if_on, mfg 0x1021),
// power_apparent (0x0B04 0x050F, VA, raw like power) and its reporting
// (optional: some firmwares refuse it, z2m swallows the error).
// NOT ported: identify (no generic identify tz) and the multiplier/divisor read
// (power stays raw, like every ZHAC electrical-measurement def).
// z2m-source: legrand.ts #WNRR15/WNRR20.
#include "definitions/_generic/_shared.hpp"
#include "definitions/legrand/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
// fz.electrical_measurement apparentPower; the generic converter stops at
// power / voltage / current.
constexpr ::zhc::generic::ZclAttrRow kApparentRows[] = { { 0x050F, "power_apparent" } };
constexpr ::zhc::generic::ZclAttrMap kApparentMap{ kApparentRows, 1 };
constexpr FzConverter kFzApparentPower = ::zhc::generic::zcl_attr_fz("haElectricalMeasurement", &kApparentMap);

const FzConverter* const kFz_WNRR15_WNRR20[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &kFzApparentPower,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::legrand::kFzClusterFc01,
};
const TzConverter* const kTz_WNRR15_WNRR20[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
    &::zhc::legrand::kTzLedInDark,
    &::zhc::legrand::kTzLedIfOn,
};
constexpr const char* kModels_WNRR15_WNRR20[] = { " Mobile outlet" };
constexpr const char* kPowerOnBehaviors[] = { "off", "on", "toggle", "previous" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_apparent", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnBehaviors, sizeof(kPowerOnBehaviors)/sizeof(kPowerOnBehaviors[0]), ExposeCategory::Config},
    {"led_in_dark", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_if_on",   ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0B04},
};

// reporting.onOff, reporting.activePower, reporting.apparentPower (try/catch).
constexpr ReportingSpec kReports_WNRR15_WNRR20[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
    {1, 0x0B04, 0x050B, 0x29, 5, 3600, 1, 0},
    {1, 0x0B04, 0x050F, 0x21, 5, 3600, 1, 0, kReportFlagOptional},
};

constexpr WhiteLabel kWhiteLabels_WNRR15_WNRR20[] = { {"Legrand", "064887"} };

extern const PreparedDefinition kDef_WNRR15_WNRR20{
    .zigbee_models=kModels_WNRR15_WNRR20, .zigbee_models_count=sizeof(kModels_WNRR15_WNRR20)/sizeof(kModels_WNRR15_WNRR20[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WNRR15/WNRR20", .vendor="Legrand",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_WNRR15_WNRR20,
    .white_labels_count=sizeof(kWhiteLabels_WNRR15_WNRR20)/sizeof(kWhiteLabels_WNRR15_WNRR20[0]),
    .from_zigbee=kFz_WNRR15_WNRR20, .from_zigbee_count=sizeof(kFz_WNRR15_WNRR20)/sizeof(kFz_WNRR15_WNRR20[0]),
    .to_zigbee=kTz_WNRR15_WNRR20, .to_zigbee_count=sizeof(kTz_WNRR15_WNRR20)/sizeof(kTz_WNRR15_WNRR20[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .reports=kReports_WNRR15_WNRR20, .reports_count=sizeof(kReports_WNRR15_WNRR20)/sizeof(kReports_WNRR15_WNRR20[0]),
};

}  // namespace zhc::devices::legrand
