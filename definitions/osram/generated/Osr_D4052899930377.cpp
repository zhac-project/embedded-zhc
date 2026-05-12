// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 4052899930377 — hand-tuned.
// Lightify pro push button controller (PBC) — four physical buttons mapped
// to endpoints l1..l4 (1..4). Each endpoint emits hold/release/toggle
// actions via genLevelCtrl commands (`ledvanceFz.pbc_level_to_action` in z2m).
// On-host we expose the four "action_l<n>" string slots and bind genLevelCtrl
// on each endpoint; the action decoder is wired up by the runtime once
// generic level→action support lands.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::osram {
namespace {

const FzConverter* const kFz_D4052899930377[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_D4052899930377[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_D4052899930377[] = { "PBC" };

constexpr ::zhc::EndpointLabel kEndpoints_D4052899930377[] = {
    {"l1", 1}, {"l2", 2}, {"l3", 3}, {"l4", 4},
};

constexpr ::zhc::Expose kExposes_D4052899930377[] = {
    { "action_l1", ::zhc::ExposeType::String, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "action_l2", ::zhc::ExposeType::String, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "action_l3", ::zhc::ExposeType::String, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "action_l4", ::zhc::ExposeType::String, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr ::zhc::BindingSpec kBindings_D4052899930377[] = {
    { 1, 0x0008 }, { 2, 0x0008 }, { 3, 0x0008 }, { 4, 0x0008 },
};

}  // namespace

extern const PreparedDefinition kDef_D4052899930377{
    .zigbee_models=kModels_D4052899930377, .zigbee_models_count=sizeof(kModels_D4052899930377)/sizeof(kModels_D4052899930377[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4052899930377", .vendor="Osram",
    .meta=nullptr,
    .exposes=kExposes_D4052899930377,
    .exposes_count=sizeof(kExposes_D4052899930377)/sizeof(kExposes_D4052899930377[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D4052899930377, .from_zigbee_count=sizeof(kFz_D4052899930377)/sizeof(kFz_D4052899930377[0]),
    .to_zigbee=kTz_D4052899930377, .to_zigbee_count=sizeof(kTz_D4052899930377)/sizeof(kTz_D4052899930377[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindings_D4052899930377,
    .bindings_count           = sizeof(kBindings_D4052899930377)/sizeof(kBindings_D4052899930377[0]),
    .endpoint_map             = kEndpoints_D4052899930377,
    .endpoint_map_count       = sizeof(kEndpoints_D4052899930377)/sizeof(kEndpoints_D4052899930377[0]),
};

}  // namespace zhc::devices::osram
