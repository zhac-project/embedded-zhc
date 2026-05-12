// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon SLC631 — auto-generated.
// Smart plug with doorbell press indicator
// z2m-source: owon.ts #SLC631.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
// z2m: 3 endpoints (l1/l2/l3) onOff + iasZoneAlarm contact alarm_2.
// We bind 0x0006 on each of the three endpoints; contact alarm comes from ssIasZone on ep1.
// TODO(zhc): multi-endpoint expose names (state_l1/l2/l3) — current ZHC schema is single-key.
const FzConverter* const kFz_SLC631[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzIasContactAlarm,
};
const TzConverter* const kTz_SLC631[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SLC631[] = { "SLC631" };

constexpr ::zhc::EndpointLabel kEndpoints_SLC631[] = { {"l1", 1}, {"l2", 2}, {"l3", 3} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",       ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"contact",     ExposeType::Binary, Access::State,    nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary, Access::State,    nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // onOff l1
    {2, 0x0006},  // onOff l2
    {3, 0x0006},  // onOff l3
    {1, 0x0500},  // ssIasZone (contact alarm_2)
};

extern const PreparedDefinition kDef_SLC631{
    .zigbee_models=kModels_SLC631, .zigbee_models_count=sizeof(kModels_SLC631)/sizeof(kModels_SLC631[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SLC631", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SLC631, .from_zigbee_count=sizeof(kFz_SLC631)/sizeof(kFz_SLC631[0]),
    .to_zigbee=kTz_SLC631, .to_zigbee_count=sizeof(kTz_SLC631)/sizeof(kTz_SLC631[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_SLC631,
    .endpoint_map_count = sizeof(kEndpoints_SLC631)/sizeof(kEndpoints_SLC631[0]),
};

}  // namespace zhc::devices::owon
