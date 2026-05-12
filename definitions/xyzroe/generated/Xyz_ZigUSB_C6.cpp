// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Xyzroe ZigUSB_C6 — hand-rewritten (was wrong-bundle: IasZone/alarm/tamper).
// Zigbee USB switch with monitoring (4 endpoints).
// z2m-source: xyzroe.ts #ZigUSB_C6.
//
// Endpoints (from m.deviceEndpoints):
//   1: switch + electrical metering (current/voltage/power)
//   2: metering (energy)
//   3: device temperature
//   4: switch (USB output) + restart timer
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::xyzroe {
namespace {
const FzConverter* const kFz_ZigUSB_C6[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzTemperature,
};
const TzConverter* const kTz_ZigUSB_C6[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_ZigUSB_C6[] = { "ZigUSB_C6" };

constexpr Expose kExp_ZigUSB_C6[] = {
    {"state",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,    nullptr, nullptr, 0},
    {"current",     ExposeType::Numeric, ::zhc::Access::State,    "A",        nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, ::zhc::Access::State,    "V",        nullptr, nullptr, 0},
    {"power",       ExposeType::Numeric, ::zhc::Access::State,    "W",        nullptr, nullptr, 0},
    {"energy",      ExposeType::Numeric, ::zhc::Access::State,    "kWh",      nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, ::zhc::Access::State,    "\xc2\xb0""C", nullptr, nullptr, 0},
    {"restart",     ExposeType::Numeric, ::zhc::Access::Set,      "s",        nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_ZigUSB_C6[] = {
    {1, 0x0006},  // genOnOff (ep1 switch)
    {1, 0x0B04},  // haElectricalMeasurement
    {1, 0x0702},  // seMetering
    {3, 0x0402},  // msTemperatureMeasurement (device temperature)
    {4, 0x0006},  // genOnOff (ep4 USB output)
};
constexpr ::zhc::EndpointLabel kEndpoints_ZigUSB_C6[] = { {"1", 1}, {"2", 2}, {"3", 3} };

}  // namespace

extern const PreparedDefinition kDef_ZigUSB_C6{
    .zigbee_models=kModels_ZigUSB_C6, .zigbee_models_count=sizeof(kModels_ZigUSB_C6)/sizeof(kModels_ZigUSB_C6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZigUSB_C6", .vendor="Xyzroe",
    .meta=nullptr, .exposes=kExp_ZigUSB_C6, .exposes_count=sizeof(kExp_ZigUSB_C6)/sizeof(kExp_ZigUSB_C6[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZigUSB_C6, .from_zigbee_count=sizeof(kFz_ZigUSB_C6)/sizeof(kFz_ZigUSB_C6[0]),
    .to_zigbee=kTz_ZigUSB_C6, .to_zigbee_count=sizeof(kTz_ZigUSB_C6)/sizeof(kTz_ZigUSB_C6[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_ZigUSB_C6, .bindings_count=sizeof(kBind_ZigUSB_C6)/sizeof(kBind_ZigUSB_C6[0]),
    .endpoint_map       = kEndpoints_ZigUSB_C6,
    .endpoint_map_count = sizeof(kEndpoints_ZigUSB_C6)/sizeof(kEndpoints_ZigUSB_C6[0]),
};

}  // namespace zhc::devices::xyzroe
