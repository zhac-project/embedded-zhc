// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon SIN-4-2-20 — hand-rewrite (per-EP exposes + manuSpec).
// Lighting relay switch — 2 endpoints (l1, l2).
//
// z2m: `m.deviceEndpoints({endpoints:{l1:1, l2:2}}) +
//       m.onOff({endpointNames:['l1','l2']}) +
//       nodonModernExtend.switchTypeOnOff({endpointName:'l1'}) +
//       nodonModernExtend.switchTypeOnOff({endpointName:'l2'})`.
//
// Endpoint suffixing — the runtime's multi-endpoint path appends `_l1`
// / `_l2` to inbound keys based on source EP and strips them from
// outbound keys before routing to the matching endpoint. So a single
// shared `kTzNodonSwitchTypeOnOff` reaches both EPs via
// `switch_type_l1` / `switch_type_l2` writes.
//
// z2m-source: nodon.ts #SIN-4-2-20.
#include "definitions/_generic/_shared.hpp"
#include "definitions/nodon/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SIN_4_2_20[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_SIN_4_2_20[] = {
    &::zhc::generic::kTzOnOff,
    &kTzNodonSwitchTypeOnOff,
};
constexpr const char* kModels_SIN_4_2_20[] = { "SIN-4-2-20" };

constexpr ::zhc::EndpointLabel kEndpoints_SIN_4_2_20[] = { {"l1", 1}, {"l2", 2} };

}  // namespace


// Hand-aligned to z2m exposes (2-endpoint variant — the `_l1`/`_l2`
// suffix is appended/stripped by the runtime EP path).
constexpr Expose kAutoExposes[] = {
    {"state_l1",       ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2",       ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_type_l1", ExposeType::Enum,   Access::StateSet, nullptr,
        "Switch type for endpoint l1.", nullptr, 0, ExposeCategory::Config},
    {"switch_type_l2", ExposeType::Enum,   Access::StateSet, nullptr,
        "Switch type for endpoint l2.", nullptr, 0, ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SIN_4_2_20{
    .zigbee_models=kModels_SIN_4_2_20, .zigbee_models_count=sizeof(kModels_SIN_4_2_20)/sizeof(kModels_SIN_4_2_20[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SIN-4-2-20", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SIN_4_2_20, .from_zigbee_count=sizeof(kFz_SIN_4_2_20)/sizeof(kFz_SIN_4_2_20[0]),
    .to_zigbee=kTz_SIN_4_2_20, .to_zigbee_count=sizeof(kTz_SIN_4_2_20)/sizeof(kTz_SIN_4_2_20[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_SIN_4_2_20,
    .endpoint_map_count = sizeof(kEndpoints_SIN_4_2_20)/sizeof(kEndpoints_SIN_4_2_20[0]),
};

}  // namespace zhc::devices::nodon
