// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sunricher SR-ZG9032A-PIR — graduated from generated/ to fix a real parity gap.
// Zigbee motion sensor + light sensor + 0-10V dimming.
// z2m-source: sunricher.ts #SR-ZG9032A-PIR — m.light({endpointNames:["1"]}),
//   m.occupancy({endpointNames:["2"]}), m.illuminance({endpointNames:["3"]}),
//   m.commandsOnOff/LevelCtrl + manuSpecific genBasic config attrs.
//
// PARITY FIX (missing core sensors): the auto-generated port lowered only the ep1
// light (state + brightness) and dropped the device's sensing half — occupancy
// (ep2, msOccupancySensing) and illuminance (ep3, msIlluminanceMeasurement). Add
// kFzOccupancy + kFzIlluminance with matching exposes + binds. The light path is
// retained unchanged. (Generic illuminance passes raw measuredValue, matching z2m.)
//
// DEFERRED (infra): the dimming_brightness_curve / daylight-threshold enumLookups
// are manuSpecific genBasic config attrs with no generic converter.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG9032A_PIR[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
};
const TzConverter* const kTz_SR_ZG9032A_PIR[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_SR_ZG9032A_PIR[] = { "HK-DIM-PIR" };

constexpr ::zhc::EndpointLabel kEndpoints_SR_ZG9032A_PIR[] = { {"1", 1}, {"2", 2}, {"3", 3} };

}  // namespace

constexpr Expose kExposes_SR_ZG9032A_PIR[] = {
    {"state",       ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",  ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State,    "lx",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_SR_ZG9032A_PIR[] = {
    {1, 0x0006},  // genOnOff (light, ep1)
    {1, 0x0008},  // genLevelCtrl (light, ep1)
    {2, 0x0406},  // msOccupancySensing (ep2)
    {3, 0x0400},  // msIlluminanceMeasurement (ep3)
};

extern const PreparedDefinition kDef_SR_ZG9032A_PIR{
    .zigbee_models=kModels_SR_ZG9032A_PIR, .zigbee_models_count=sizeof(kModels_SR_ZG9032A_PIR)/sizeof(kModels_SR_ZG9032A_PIR[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG9032A-PIR", .vendor="Sunricher",
    .meta=nullptr, .exposes=kExposes_SR_ZG9032A_PIR, .exposes_count=sizeof(kExposes_SR_ZG9032A_PIR)/sizeof(kExposes_SR_ZG9032A_PIR[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SR_ZG9032A_PIR, .from_zigbee_count=sizeof(kFz_SR_ZG9032A_PIR)/sizeof(kFz_SR_ZG9032A_PIR[0]),
    .to_zigbee=kTz_SR_ZG9032A_PIR, .to_zigbee_count=sizeof(kTz_SR_ZG9032A_PIR)/sizeof(kTz_SR_ZG9032A_PIR[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_SR_ZG9032A_PIR,.bindings_count=sizeof(kBindings_SR_ZG9032A_PIR)/sizeof(kBindings_SR_ZG9032A_PIR[0]),
    .endpoint_map       = kEndpoints_SR_ZG9032A_PIR,
    .endpoint_map_count = sizeof(kEndpoints_SR_ZG9032A_PIR)/sizeof(kEndpoints_SR_ZG9032A_PIR[0]),
};

}  // namespace zhc::devices::sunricher
