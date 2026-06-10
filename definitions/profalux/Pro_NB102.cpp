// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Profalux NB102 — empty cover remote (parity fix).
//
// z2m defines NB102 with `fromZigbee: []`, `toZigbee: []`, `exposes: []`:
// it is an older cover *remote* bound directly to the cover, so it never
// talks to the coordinator and exposes nothing in z2m. The auto-port
// hallucinated a phantom on/off `state` plus a `battery`/`voltage` bundle
// and wired kFzOnOff + kFzBattery with genOnOff/genPowerCfg bindings — none
// of which exist in z2m for this model. Stripped to an empty definition so
// it matches z2m exactly (model present for identification, no decoders,
// no exposes, no bindings).
// z2m-source: profalux.ts #NB102.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::profalux {
namespace {

constexpr const char* kModels_NB102[] = { "MAI-ZTS" };

}  // namespace

extern const PreparedDefinition kDef_NB102{
    .zigbee_models=kModels_NB102, .zigbee_models_count=sizeof(kModels_NB102)/sizeof(kModels_NB102[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NB102", .vendor="Profalux",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::profalux
