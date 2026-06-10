// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Slv 1002994 — graduated override (phantom bundle removed).
// VALETO remote (binds directly to the controlled device)
// z2m-source: slv.ts #1002994 — fromZigbee:[], toZigbee:[], exposes:[].
// Gap: the auto-port invented a controllable on/off state plus battery +
// voltage exposes/decoders (class e phantom bundle). In z2m this remote is
// a pure binder: it relays commands straight to the bound luminaire and
// exposes NOTHING to the gateway. Match z2m exactly — no exposes, no fz/tz,
// no bindings (z2m declares no configure).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slv {
namespace {

constexpr const char* kModels_D1002994[] = { "1002994" };

}  // namespace

extern const PreparedDefinition kDef_D1002994{
    .zigbee_models=kModels_D1002994, .zigbee_models_count=sizeof(kModels_D1002994)/sizeof(kModels_D1002994[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1002994", .vendor="Slv",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::slv
