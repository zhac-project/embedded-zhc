// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linkind ZL100050004 — hand-rewritten 2026-04-28q.
// Zigbee LED 7.4W BR30 bulb E26, dimmable & tunable.
// z2m bundle: m.light({colorTemp: {range: undefined}}) → CTLight.
// z2m-source: linkind.ts #ZL100050004.
#include "definitions/_generic/_shared.hpp"
#include "definitions/linkind/_shared.hpp"

namespace zhc::devices::linkind {
namespace {

constexpr const char* kModels_ZL100050004[] = { "ZBT-CCTLight-BR300107" };

}  // namespace

extern const PreparedDefinition kDef_ZL100050004{
    .zigbee_models=kModels_ZL100050004, .zigbee_models_count=sizeof(kModels_ZL100050004)/sizeof(kModels_ZL100050004[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZL100050004", .vendor="Linkind",
    .meta=nullptr, .exposes=kExposesLinkindCTLight, .exposes_count=kExposesLinkindCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLinkindCTLight, .from_zigbee_count=kFzLinkindCTLightCount,
    .to_zigbee=kTzLinkindCTLight, .to_zigbee_count=kTzLinkindCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsLinkindCTLight, .bindings_count=kBindingsLinkindCTLightCount,
};

}  // namespace zhc::devices::linkind
