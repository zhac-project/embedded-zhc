// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SPP02G — hand-rewritten (multi-EP + electricalMeasurements).
// Double power point.
//
// z2m extend: tuya.modernExtend.tuyaOnOff({powerOutageMemory:true,
//   electricalMeasurements:true, endpoints:["left","right"]})
//   + endpoint:(d)=>({left:1,right:2})
//
// 2026-04-28p MERCATOR sweep — fixed up from on/off-only stub.
// powerOutageMemory + per-EP power-on behaviour writes are not yet
// surfaced (no kTzPowerOnBehavior in generic for this multi-EP shape;
// see runtime gap in MERCATOR_PARITY.md).
//
// z2m-source: mercator.ts #SPP02G.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SPP02G[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SPP02G[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SPP02G[] = { "TS011F" };
constexpr const char* kManus_SPP02G[] = { "_TZ3210_raqjcxo5" };

constexpr Expose kExposes_SPP02G[] = {
    { "state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0 },
    { "power",   ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0 },
    { "energy",  ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SPP02G[] = {
    { 1, 0x0006 }, { 2, 0x0006 },
    { 1, 0x0702 }, { 1, 0x0B04 },  // metering+electrical only on EP1 per z2m configure()
};

constexpr ::zhc::EndpointLabel kEndpoints_SPP02G[] = {
    { "left",  1 },
    { "right", 2 },
};

}  // namespace

extern const PreparedDefinition kDef_SPP02G{
    .zigbee_models=kModels_SPP02G, .zigbee_models_count=sizeof(kModels_SPP02G)/sizeof(kModels_SPP02G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SPP02G, .manufacturer_names_count=sizeof(kManus_SPP02G)/sizeof(kManus_SPP02G[0]),
    .model="SPP02G", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SPP02G, .exposes_count=sizeof(kExposes_SPP02G)/sizeof(kExposes_SPP02G[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SPP02G, .from_zigbee_count=sizeof(kFz_SPP02G)/sizeof(kFz_SPP02G[0]),
    .to_zigbee=kTz_SPP02G, .to_zigbee_count=sizeof(kTz_SPP02G)/sizeof(kTz_SPP02G[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SPP02G, .bindings_count=sizeof(kBindings_SPP02G)/sizeof(kBindings_SPP02G[0]),
    .endpoint_map       = kEndpoints_SPP02G,
    .endpoint_map_count = sizeof(kEndpoints_SPP02G)/sizeof(kEndpoints_SPP02G[0]),
};

}  // namespace zhc::devices::mercator
