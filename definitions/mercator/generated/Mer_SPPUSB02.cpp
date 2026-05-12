// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SPPUSB02 — hand-rewritten (multi-EP + electricalMeasurements).
// Double power point with USB.
//
// z2m extend: tuya.modernExtend.tuyaOnOff({powerOutageMemory:true,
//   electricalMeasurements:true, endpoints:["left","right"]})
//   + endpoint:(d)=>({left:1,right:2})
//
// 2026-04-28p MERCATOR sweep — fixed up from on/off-only stub.
//
// z2m-source: mercator.ts #SPPUSB02.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SPPUSB02[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SPPUSB02[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SPPUSB02[] = { "TS011F" };
constexpr const char* kManus_SPPUSB02[] = { "_TZ3210_pfbzs1an" };

constexpr Expose kExposes_SPPUSB02[] = {
    { "state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0 },
    { "power",   ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0 },
    { "energy",  ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SPPUSB02[] = {
    { 1, 0x0006 }, { 2, 0x0006 },
    { 1, 0x0702 }, { 1, 0x0B04 },
};

constexpr ::zhc::EndpointLabel kEndpoints_SPPUSB02[] = {
    { "left",  1 },
    { "right", 2 },
};

}  // namespace

extern const PreparedDefinition kDef_SPPUSB02{
    .zigbee_models=kModels_SPPUSB02, .zigbee_models_count=sizeof(kModels_SPPUSB02)/sizeof(kModels_SPPUSB02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SPPUSB02, .manufacturer_names_count=sizeof(kManus_SPPUSB02)/sizeof(kManus_SPPUSB02[0]),
    .model="SPPUSB02", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SPPUSB02, .exposes_count=sizeof(kExposes_SPPUSB02)/sizeof(kExposes_SPPUSB02[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SPPUSB02, .from_zigbee_count=sizeof(kFz_SPPUSB02)/sizeof(kFz_SPPUSB02[0]),
    .to_zigbee=kTz_SPPUSB02, .to_zigbee_count=sizeof(kTz_SPPUSB02)/sizeof(kTz_SPPUSB02[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SPPUSB02, .bindings_count=sizeof(kBindings_SPPUSB02)/sizeof(kBindings_SPPUSB02[0]),
    .endpoint_map       = kEndpoints_SPPUSB02,
    .endpoint_map_count = sizeof(kEndpoints_SPPUSB02)/sizeof(kEndpoints_SPPUSB02[0]),
};

}  // namespace zhc::devices::mercator
