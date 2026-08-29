// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara WP-P09D Wall outlet H2 UK (lumi.plug.aeu002).
// New upstream in z2m v26.100.0.
//
// Three switched loads on endpoints 1, 2 and 3 — z2m labels them "1", "2" and
// "usb", and this def keeps those labels so the published keys match
// (`state_1`, `state_2`, `state_usb`).
//
// The metering endpoints DO NOT line up with the switching ones, which is the
// trap on this device and worth restating from upstream's own comment:
//   endpoint 1 measures the WHOLE outlet,
//   endpoint 2 measures socket 1 PLUS the USB ports combined,
//   endpoint 3 measures socket 2.
// So `power_2` is not "the power of what state_2 controls". The labels are
// preserved verbatim rather than renamed, because renaming them would diverge
// from z2m and from the vendor app without making the mapping any less
// confusing. Anyone automating on these should read this comment first.
//
// This model reports no voltage (z2m passes `voltage: false`) and has no
// device-temperature sensor, so neither is exposed.
//
// DEFERRED: `lumiMultiClick` (per-socket button click-mode config) and
// `lumiAction` — both ride the 0xFCC0 manufacturer cluster with a per-model
// attribute layout, and the switching function is complete without them.
//
// z2m-source: lumi.ts #WP-P09D.
#include "definitions/lumi/_shared.hpp"
#include "definitions/lumi/factories.hpp"

namespace zhc::devices::lumi {
namespace {

const FzConverter* const kFz_WP_P09D[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::lumi::kFzLumiElectricityMeter,
    &::zhc::lumi::kFzLumiManuSpecific,
};
const TzConverter* const kTz_WP_P09D[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_WP_P09D[] = { "lumi.plug.aeu002" };

constexpr ::zhc::EndpointLabel kEndpoints_WP_P09D[] = {
    {"1", 1}, {"2", 2}, {"usb", 3},
};

constexpr const char* kPowerOutageOpts[] = { "on", "previous", "off" };

constexpr Expose kExposes_WP_P09D[] = {
    {"state_1",   ExposeType::Binary,  Access::StateSet, nullptr, "Socket 1", nullptr, 0},
    {"state_2",   ExposeType::Binary,  Access::StateSet, nullptr, "Socket 2", nullptr, 0},
    {"state_usb", ExposeType::Binary,  Access::StateSet, nullptr, "USB ports", nullptr, 0},
    {"power_1",   ExposeType::Numeric, Access::State, "W",
     "Total power consumption of the outlet", nullptr, 0},
    {"power_2",   ExposeType::Numeric, Access::State, "W",
     "Combined power consumption of socket 1 and the USB ports", nullptr, 0},
    {"power_usb", ExposeType::Numeric, Access::State, "W",
     "Power consumption of socket 2", nullptr, 0},
    {"energy",    ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power_outage_memory", ExposeType::Enum, Access::StateSet, nullptr,
     "State after a power outage", kPowerOutageOpts,
     sizeof(kPowerOutageOpts)/sizeof(kPowerOutageOpts[0]), ExposeCategory::Config},
    {"power_outage_count",  ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_WP_P09D[] = {
    {1, 0x0000},   // genBasic — lumi 0xFCC0 rides here
    {1, 0x0006},   // genOnOff — socket 1
    {2, 0x0006},   // genOnOff — socket 2
    {3, 0x0006},   // genOnOff — USB
    {1, 0x0B04},   // haElectricalMeasurement — whole outlet
    {2, 0x0B04},   // haElectricalMeasurement — socket 1 + USB
    {3, 0x0B04},   // haElectricalMeasurement — socket 2
    {1, 0x0702},   // seMetering — energy
};

}  // namespace

extern const PreparedDefinition kDefWP_P09D{
    .zigbee_models=kModels_WP_P09D,
    .zigbee_models_count=sizeof(kModels_WP_P09D)/sizeof(kModels_WP_P09D[0]),
    .model="WP-P09D", .vendor="Aqara",
    .meta=nullptr,
    .exposes=kExposes_WP_P09D,
    .exposes_count=sizeof(kExposes_WP_P09D)/sizeof(kExposes_WP_P09D[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WP_P09D, .from_zigbee_count=sizeof(kFz_WP_P09D)/sizeof(kFz_WP_P09D[0]),
    .to_zigbee=kTz_WP_P09D, .to_zigbee_count=sizeof(kTz_WP_P09D)/sizeof(kTz_WP_P09D[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_WP_P09D,
    .bindings_count=sizeof(kBindings_WP_P09D)/sizeof(kBindings_WP_P09D[0]),
    .endpoint_map       = kEndpoints_WP_P09D,
    .endpoint_map_count = sizeof(kEndpoints_WP_P09D)/sizeof(kEndpoints_WP_P09D[0]),
    .power_source_override = 0x01,   // ZCL Basic 0x0007 = Mains (single phase)
};

}  // namespace zhc::devices::lumi
