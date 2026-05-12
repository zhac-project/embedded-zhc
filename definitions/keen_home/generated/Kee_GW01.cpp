// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: KeenHome GW01 — signal repeater. Mains-powered router with no
// user-controllable state. z2m wires only `fz.linkquality_from_basic`
// (which republishes the inbound LQ from any genBasic frame) and binds
// genBasic so the repeater periodically reports modelId. ZHC tracks
// linkquality on every frame regardless, so no Fz is needed.
// The previous auto-generated port wired kFzOnOff/kTzOnOff and bound
// genOnOff (0x0006) — a stub-shadow of an irrelevant cluster. Removed.
// z2m-source: keen_home.ts #GW01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::keen_home {
namespace {
constexpr const char* kModels_GW01[] = { "GW01-001-MP-1.0" };

constexpr BindingSpec kBind_GW01[] = {
    {1, 0x0000},   // genBasic — modelId reporting
};
}  // namespace

extern const PreparedDefinition kDef_GW01{
    .zigbee_models=kModels_GW01,
    .zigbee_models_count=sizeof(kModels_GW01)/sizeof(kModels_GW01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GW01", .vendor="KeenHome",
    .meta=nullptr,
    .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr,   .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_GW01,
    .bindings_count=sizeof(kBind_GW01)/sizeof(kBind_GW01[0]),
};

}  // namespace zhc::devices::keen_home
