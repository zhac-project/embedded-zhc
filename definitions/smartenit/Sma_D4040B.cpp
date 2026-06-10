// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Smartenit 4040B — wireless metering 30A dual-load switch.
// z2m-source: smartenit.ts #4040B — fz.on_off, fz.metering;
//             endpoint: {l1: 1, l2: 2}; exposes switch(l1)+switch(l2)+power+energy.
//
// Bug fixed: the Tier-1 auto-port omitted the {l1:1, l2:2} endpoint_map,
// so genOnOff reports from both loads collided on the bare "state" key
// (last-writer-wins → the two relays were indistinguishable). z2m maps
// l1→ep1, l2→ep2; with the endpoint_map the dispatcher now suffixes the
// per-load switch to state_l1 / state_l2. power/energy stay device-global
// (z2m's untagged e.power()/e.energy()), kept un-suffixed via the
// dispatch blocklist. Also bind genOnOff + seMetering on ep2 to match
// z2m configure (which binds both clusters on endpoints 1 and 2).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartenit {
namespace {
const FzConverter* const kFz_D4040B[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_D4040B[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_D4040B[] = { "ZBMLC30" };

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff   ep1 (l1)
    {1, 0x0702},  // seMetering ep1
    {2, 0x0006},  // genOnOff   ep2 (l2)
    {2, 0x0702},  // seMetering ep2
};

constexpr ::zhc::EndpointLabel kEndpoints_D4040B[] = { {"l1", 1}, {"l2", 2} };

}  // namespace

extern const PreparedDefinition kDef_D4040B{
    .zigbee_models=kModels_D4040B, .zigbee_models_count=sizeof(kModels_D4040B)/sizeof(kModels_D4040B[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4040B", .vendor="Smartenit",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D4040B, .from_zigbee_count=sizeof(kFz_D4040B)/sizeof(kFz_D4040B[0]),
    .to_zigbee=kTz_D4040B, .to_zigbee_count=sizeof(kTz_D4040B)/sizeof(kTz_D4040B[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_D4040B,
    .endpoint_map_count = sizeof(kEndpoints_D4040B)/sizeof(kEndpoints_D4040B[0]),
};

}  // namespace zhc::devices::smartenit
