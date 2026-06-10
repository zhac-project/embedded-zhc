// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Smartenit 4040B — wireless metering 30A dual-load switch.
// z2m-source: smartenit.ts #4040B — fz.on_off, fz.metering;
//             endpoint: {l1: 1, l2: 2}; exposes switch(l1)+switch(l2)+power+energy.
//
// Real gap: the Tier-1 auto-port dropped the seMetering bind on ep2. Restored
// genOnOff + seMetering binds on ep1 AND ep2 (z2m configure binds both clusters
// on both endpoints).
//
// DEFERRED (INFRA, dual-load state split): z2m maps l1→ep1, l2→ep2 with
// switch().withEndpoint() (per-load state) BUT keeps power/energy as a single
// untagged e.power()/e.energy(). The runtime suffix rewrite can only suffix
// ALL non-global keys or none — there is no per-def metering-skip hook — so an
// endpoint_map here would wrongly suffix power/energy to power_l1/energy_l1
// (diverging from z2m's bare keys; only `voltage` is in kAlwaysGlobalKeys[]).
// The def therefore ships MAPLESS: a single bare `state` (the two loads
// collapse, last-writer-wins) + bare power/energy (correct), identical to the
// structurally-equal Makegood GPO01/AUZG01 and Honyar HY0157. The per-load
// state split waits on the dispatch infra.
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
};

}  // namespace zhc::devices::smartenit
