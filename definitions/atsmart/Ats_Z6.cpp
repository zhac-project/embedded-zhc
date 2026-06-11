// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Atsmart Z6 — graduated parity override.
// 3 gang smart wall switch (no neutral wire)
// z2m-source: atsmart.ts #Z6 —
//   m.deviceEndpoints({left:1, center:2, right:3}) +
//   m.onOff({endpointNames:["left","center","right"]}).
//
// Bug fixed (MISSING per-endpoint bindings + collapsed exposes): the
// generated Tier-1 def carried the endpoint_map {left:1, center:2, right:3}
// and kFzOnOff, but (a) bound genOnOff (0x0006) on endpoint 1 ONLY, so the
// center/right gangs never had their reporting configured, and (b) collapsed
// the three per-gang exposes into a single bare "state". With the
// endpoint_map present, dispatch suffixes inbound onOff keys to
// state_<label> — the surface keys are state_left / state_center /
// state_right, so the bare "state" expose matched no runtime key. z2m
// m.onOff({endpointNames:[...]}) binds genOnOff on all three endpoints.
// Fix: bind 0x0006 on endpoints 1/2/3 and declare the per-gang exposes.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::atsmart {
namespace {
const FzConverter* const kFz_Z6[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_Z6[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_Z6[] = { "Z601", "Z602", "Z603", "Z604" };

constexpr ::zhc::EndpointLabel kEndpoints_Z6[] = { {"left", 1}, {"center", 2}, {"right", 3} };

}  // namespace


// Per-gang surface: dispatch suffixes runtime onOff keys to state_<label>
// from the endpoint_map, so the exposes are declared suffixed (matching the
// Lonsonho/EasyIoT multi-gang pattern), NOT a single bare "state".
constexpr Expose kExposes_Z6[] = {
    {"state_left",   ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_center", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

// z2m m.onOff({endpointNames:["left","center","right"]}) binds genOnOff
// (0x0006) on every named endpoint, not just endpoint 1.
constexpr BindingSpec kBindings_Z6[] = {
    {1, 0x0006}, {2, 0x0006}, {3, 0x0006},
};

extern const PreparedDefinition kDef_Z6{
    .zigbee_models=kModels_Z6, .zigbee_models_count=sizeof(kModels_Z6)/sizeof(kModels_Z6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Z6", .vendor="Atsmart",
    .meta=nullptr, .exposes=kExposes_Z6, .exposes_count=sizeof(kExposes_Z6)/sizeof(kExposes_Z6[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Z6, .from_zigbee_count=sizeof(kFz_Z6)/sizeof(kFz_Z6[0]),
    .to_zigbee=kTz_Z6, .to_zigbee_count=sizeof(kTz_Z6)/sizeof(kTz_Z6[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_Z6, .bindings_count=sizeof(kBindings_Z6)/sizeof(kBindings_Z6[0]),
    .endpoint_map       = kEndpoints_Z6,
    .endpoint_map_count = sizeof(kEndpoints_Z6)/sizeof(kEndpoints_Z6[0]),
};

}  // namespace zhc::devices::atsmart
