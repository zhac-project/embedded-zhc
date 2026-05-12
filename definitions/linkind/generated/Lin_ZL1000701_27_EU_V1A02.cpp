// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZL1000701-27-EU-V1A02 — hand-rewritten 2026-04-28q.
// Zigbee A60 filament bulb 6.3W.
// z2m bundle: m.light() → plain dimmable.
// z2m-source: linkind.ts #ZL1000701-27-EU-V1A02.
#include "definitions/_generic/_shared.hpp"
#include "definitions/linkind/_shared.hpp"

namespace zhc::devices::linkind {
namespace {

constexpr const char* kModels_ZL1000701_27_EU_V1A02[] = { "ZBT-DIMLight-D0120" };

}  // namespace

extern const PreparedDefinition kDef_ZL1000701_27_EU_V1A02{
    .zigbee_models=kModels_ZL1000701_27_EU_V1A02,
    .zigbee_models_count=sizeof(kModels_ZL1000701_27_EU_V1A02)/sizeof(kModels_ZL1000701_27_EU_V1A02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZL1000701-27-EU-V1A02", .vendor="Linkind",
    .meta=nullptr, .exposes=kExposesLinkindLight, .exposes_count=kExposesLinkindLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLinkindLight, .from_zigbee_count=kFzLinkindLightCount,
    .to_zigbee=kTzLinkindLight, .to_zigbee_count=kTzLinkindLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsLinkindLight, .bindings_count=kBindingsLinkindLightCount,
};

}  // namespace zhc::devices::linkind
