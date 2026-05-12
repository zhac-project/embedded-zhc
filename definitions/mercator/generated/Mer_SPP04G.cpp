// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SPP04G — hand-rewritten (multi-EP + electricalMeasurements).
// Quad power point.
//
// z2m extend: tuya.modernExtend.tuyaOnOff({powerOutageMemory:true,
//   electricalMeasurements:true, endpoints:["left","right"]})
//   + endpoint:(d)=>({left:1,right:2})
//
// Note: model name says "Quad" but z2m endpoint map is left/right
// (EP1+EP2). Mirroring z2m exactly so user-visible keys match.
//
// 2026-04-28p MERCATOR sweep — fixed up from on/off-only stub.
//
// z2m-source: mercator.ts #SPP04G.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SPP04G[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SPP04G[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SPP04G[] = { "TS011F" };
constexpr const char* kManus_SPP04G[] = { "_TZ3210_yvxjawlt" };

constexpr Expose kExposes_SPP04G[] = {
    { "state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0 },
    { "power",   ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0 },
    { "energy",  ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SPP04G[] = {
    { 1, 0x0006 }, { 2, 0x0006 },
    { 1, 0x0702 }, { 1, 0x0B04 },
};

constexpr ::zhc::EndpointLabel kEndpoints_SPP04G[] = {
    { "left",  1 },
    { "right", 2 },
};

}  // namespace

extern const PreparedDefinition kDef_SPP04G{
    .zigbee_models=kModels_SPP04G, .zigbee_models_count=sizeof(kModels_SPP04G)/sizeof(kModels_SPP04G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SPP04G, .manufacturer_names_count=sizeof(kManus_SPP04G)/sizeof(kManus_SPP04G[0]),
    .model="SPP04G", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SPP04G, .exposes_count=sizeof(kExposes_SPP04G)/sizeof(kExposes_SPP04G[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SPP04G, .from_zigbee_count=sizeof(kFz_SPP04G)/sizeof(kFz_SPP04G[0]),
    .to_zigbee=kTz_SPP04G, .to_zigbee_count=sizeof(kTz_SPP04G)/sizeof(kTz_SPP04G[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SPP04G, .bindings_count=sizeof(kBindings_SPP04G)/sizeof(kBindings_SPP04G[0]),
    .endpoint_map       = kEndpoints_SPP04G,
    .endpoint_map_count = sizeof(kEndpoints_SPP04G)/sizeof(kEndpoints_SPP04G[0]),
};

}  // namespace zhc::devices::mercator
