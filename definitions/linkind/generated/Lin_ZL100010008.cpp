// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZL100010008 — hand-rewritten 2026-04-28q.
// Zigbee LED 9W 2700K A19 bulb, dimmable.
// z2m bundle: m.light() → plain dimmable.
// z2m-source: linkind.ts #ZL100010008.
#include "definitions/_generic/_shared.hpp"
#include "definitions/linkind/_shared.hpp"

namespace zhc::devices::linkind {
namespace {

constexpr const char* kModels_ZL100010008[] = { "ZBT-DIMLight-GLS0010" };

}  // namespace

extern const PreparedDefinition kDef_ZL100010008{
    .zigbee_models=kModels_ZL100010008, .zigbee_models_count=sizeof(kModels_ZL100010008)/sizeof(kModels_ZL100010008[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZL100010008", .vendor="Linkind",
    .meta=nullptr, .exposes=kExposesLinkindLight, .exposes_count=kExposesLinkindLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLinkindLight, .from_zigbee_count=kFzLinkindLightCount,
    .to_zigbee=kTzLinkindLight, .to_zigbee_count=kTzLinkindLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsLinkindLight, .bindings_count=kBindingsLinkindLightCount,
};

}  // namespace zhc::devices::linkind
