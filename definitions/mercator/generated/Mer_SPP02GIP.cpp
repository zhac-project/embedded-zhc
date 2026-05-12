// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SPP02GIP — hand-rewritten (multi-EP + electricalMeasurements).
// Double power point IP54.
//
// z2m extend: tuya.modernExtend.tuyaOnOff({powerOutageMemory:true,
//   electricalMeasurements:true, endpoints:["left","right"]})
//   + endpoint:(d)=>({left:1,right:2})
//
// 2026-04-28p MERCATOR sweep — fixed up from on/off-only stub.
//
// z2m-source: mercator.ts #SPP02GIP.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SPP02GIP[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SPP02GIP[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SPP02GIP[] = { "TS011F" };
constexpr const char* kManus_SPP02GIP[] = { "_TZ3210_7jnk7l3k" };

constexpr Expose kExposes_SPP02GIP[] = {
    { "state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0 },
    { "power",   ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0 },
    { "energy",  ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SPP02GIP[] = {
    { 1, 0x0006 }, { 2, 0x0006 },
    { 1, 0x0702 }, { 1, 0x0B04 },
};

constexpr ::zhc::EndpointLabel kEndpoints_SPP02GIP[] = {
    { "left",  1 },
    { "right", 2 },
};

}  // namespace

extern const PreparedDefinition kDef_SPP02GIP{
    .zigbee_models=kModels_SPP02GIP, .zigbee_models_count=sizeof(kModels_SPP02GIP)/sizeof(kModels_SPP02GIP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SPP02GIP, .manufacturer_names_count=sizeof(kManus_SPP02GIP)/sizeof(kManus_SPP02GIP[0]),
    .model="SPP02GIP", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SPP02GIP, .exposes_count=sizeof(kExposes_SPP02GIP)/sizeof(kExposes_SPP02GIP[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SPP02GIP, .from_zigbee_count=sizeof(kFz_SPP02GIP)/sizeof(kFz_SPP02GIP[0]),
    .to_zigbee=kTz_SPP02GIP, .to_zigbee_count=sizeof(kTz_SPP02GIP)/sizeof(kTz_SPP02GIP[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SPP02GIP, .bindings_count=sizeof(kBindings_SPP02GIP)/sizeof(kBindings_SPP02GIP[0]),
    .endpoint_map       = kEndpoints_SPP02GIP,
    .endpoint_map_count = sizeof(kEndpoints_SPP02GIP)/sizeof(kEndpoints_SPP02GIP[0]),
};

}  // namespace zhc::devices::mercator
