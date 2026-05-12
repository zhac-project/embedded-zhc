// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: KeenHome GW02 — signal repeater. Identical to GW01 (same z2m
// definition shape). Only `fz.linkquality_from_basic` upstream; no
// user-controllable state. The previous auto-generated port wired
// kFzOnOff/kTzOnOff and bound genOnOff — a stub-shadow of an
// irrelevant cluster. Removed; binds genBasic instead.
// z2m-source: keen_home.ts #GW02.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::keen_home {
namespace {
constexpr const char* kModels_GW02[] = { "GW02-001-MP-1.0" };

constexpr BindingSpec kBind_GW02[] = {
    {1, 0x0000},   // genBasic — modelId reporting
};
}  // namespace

extern const PreparedDefinition kDef_GW02{
    .zigbee_models=kModels_GW02,
    .zigbee_models_count=sizeof(kModels_GW02)/sizeof(kModels_GW02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GW02", .vendor="KeenHome",
    .meta=nullptr,
    .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr,   .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_GW02,
    .bindings_count=sizeof(kBind_GW02)/sizeof(kBind_GW02[0]),
};

}  // namespace zhc::devices::keen_home
