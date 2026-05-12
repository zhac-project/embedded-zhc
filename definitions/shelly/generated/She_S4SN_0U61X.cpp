// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Shelly S4SN-0U61X — Presence Gen4 Zigbee, 10-zone occupancy.
//
// 10 endpoints (1..10), each binds msOccupancySensing (0x0406) for one
// zone. Per-zone exposes flow through the multi-EP suffix runtime
// (landed 2026-04-28k):
//   * Inbound: dispatch_from_zigbee rewrites emitted keys with
//     `_<label>` based on src_endpoint. Labels "1".."10" mirror z2m's
//     `m.deviceEndpoints({endpoints: {"1":1, ..., "10":10}})`.
//   * Outbound: dispatch_and_send strips `_<label>` and routes to the
//     corresponding endpoint.
//
// Custom cluster `shellyLightLevel` (0xFC21, manuCode `SHELLY` 0x1463)
// carries a 3-state light-level enum + dark/bright lux thresholds.
// Decoded path requires the manuSpec read/write plumbing already used
// by shelly's other Gen4 ports — wired below for binding completeness;
// runtime decode of the enum requires a `shelly/_shared` module
// (tracked as gap #29 in PARITY_SUMMARY).
//
// Unblocked 2026-04-28p post-sweep: `kFzOccupancy` landed in
// `_generic/_shared`. The 10 occupancy zones now decode and surface
// as `occupancy_1`..`occupancy_10` via the multi-EP suffix runtime.
// Light-level cluster `shellyLightLevel` (0xFC21) is still STATE-only
// pending `shelly/_shared`.
//
// z2m-source: shelly.ts #S4SN-0U61X (lines 1534-1597).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::shelly {
namespace {
const FzConverter* const kFz_S4SN_0U61X[] = {
    &::zhc::generic::kFzOccupancy,
};

constexpr const char* kModels_S4SN_0U61X[] = { "Presence" };
constexpr const char* kManus_S4SN_0U61X[]  = { "Shelly" };

// 10-zone occupancy fan-out. Labels match z2m's deviceEndpoints map
// verbatim ("1".."10" → EPs 1..10). Multi-EP runtime suffixes emitted
// keys with `_<label>` so SPA gets `occupancy_1`/.../`occupancy_10`.
constexpr ::zhc::EndpointLabel kEndpoints_S4SN_0U61X[] = {
    {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5},
    {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}, {"10", 10},
};

}  // namespace


// One expose per zone — declared statically so the SPA knows there
// are 10 separate occupancy slots. Runtime fills them via the suffix
// rewrite once `kFzOccupancy` lands.
constexpr Expose kAutoExposes[] = {
    {"occupancy_1",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_2",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_3",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_4",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_5",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_6",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_7",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_8",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_9",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_10", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    // Light-level cluster — STATE-only until shelly/_shared lands.
    {"light_level",      ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"dark_threshold",   ExposeType::Numeric, Access::StateSet, "lx",    nullptr, nullptr, 0},
    {"bright_threshold", ExposeType::Numeric, Access::StateSet, "lx",    nullptr, nullptr, 0},
};

// Bind msOccupancySensing on every endpoint so the device starts
// publishing zone reports immediately on join. shellyLightLevel
// (0xFC21) bound on EP 1 — its attrs are read on demand, no reporting
// configured (matches z2m's `reporting: false`).
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0406}, {2, 0x0406}, {3, 0x0406}, {4, 0x0406}, {5, 0x0406},
    {6, 0x0406}, {7, 0x0406}, {8, 0x0406}, {9, 0x0406}, {10, 0x0406},
    {1, 0xFC21},  // shellyLightLevel — read-only access surface
};

extern const PreparedDefinition kDef_S4SN_0U61X{
    .zigbee_models=kModels_S4SN_0U61X, .zigbee_models_count=sizeof(kModels_S4SN_0U61X)/sizeof(kModels_S4SN_0U61X[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_S4SN_0U61X, .manufacturer_names_count=sizeof(kManus_S4SN_0U61X)/sizeof(kManus_S4SN_0U61X[0]),
    .model="S4SN-0U61X", .vendor="Shelly",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S4SN_0U61X, .from_zigbee_count=sizeof(kFz_S4SN_0U61X)/sizeof(kFz_S4SN_0U61X[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_S4SN_0U61X,
    .endpoint_map_count = sizeof(kEndpoints_S4SN_0U61X)/sizeof(kEndpoints_S4SN_0U61X[0]),
};

}  // namespace zhc::devices::shelly
