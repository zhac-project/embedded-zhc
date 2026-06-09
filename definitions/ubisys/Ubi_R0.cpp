// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ubisys R0 — Zigbee router / range extender with no
// user-controllable state. z2m declares `exposes: []` and wires only
// `fz.linkquality_from_basic` (republishes inbound LQ from any genBasic
// frame); it has no toZigbee and no genOnOff cluster.
//
// Parity fix (real gap): the auto-generated port wired kFzOnOff /
// kTzOnOff and exposed a `state` on/off — a stub-shadow of a cluster the
// router does not implement, so `state` was a dead, uncontrollable
// expose (bug class e: wrong default bundle on a non-switch). ZHC tracks
// linkquality on every frame regardless, so no Fz is needed. Mirrors the
// KeenHome GW01 repeater fix.
// z2m-source: ubisys.ts #R0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ubisys {
namespace {
constexpr const char* kModels_R0[] = { "R0 (5501)" };

constexpr BindingSpec kBind_R0[] = {
    {1, 0x0000},   // genBasic — modelId reporting
};
}  // namespace

extern const PreparedDefinition kDef_R0{
    .zigbee_models=kModels_R0, .zigbee_models_count=sizeof(kModels_R0)/sizeof(kModels_R0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="R0", .vendor="Ubisys",
    .meta=nullptr,
    .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr,   .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_R0, .bindings_count=sizeof(kBind_R0)/sizeof(kBind_R0[0]),
};

}  // namespace zhc::devices::ubisys
