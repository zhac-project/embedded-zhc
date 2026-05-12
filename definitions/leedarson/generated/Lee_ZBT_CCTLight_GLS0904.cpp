// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson ZBT-CCTLight-GLS0904 — hand-rewritten 2026-04-29.
// LED E27 tunable white
// z2m-source: leedarson.ts #ZBT-CCTLight-GLS0904.
// Mapping: m.light({colorTemp: {range: [153, 370]}})
//          -> kFz/kTz/kExposes/kBindings LeedarsonCTLight (OnOff+Brightness+ColorTemp).
#include "definitions/_generic/_shared.hpp"
#include "definitions/leedarson/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
constexpr const char* kModels_ZBT_CCTLight_GLS0904[] = { "ZBT-CCTLight-GLS0904" };
}  // namespace

extern const PreparedDefinition kDef_ZBT_CCTLight_GLS0904{
    .zigbee_models=kModels_ZBT_CCTLight_GLS0904, .zigbee_models_count=sizeof(kModels_ZBT_CCTLight_GLS0904)/sizeof(kModels_ZBT_CCTLight_GLS0904[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZBT-CCTLight-GLS0904", .vendor="Leedarson",
    .meta=nullptr,
    .exposes=kExposesLeedarsonCTLight, .exposes_count=kExposesLeedarsonCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLeedarsonCTLight, .from_zigbee_count=kFzLeedarsonCTLightCount,
    .to_zigbee=kTzLeedarsonCTLight, .to_zigbee_count=kTzLeedarsonCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsLeedarsonCTLight, .bindings_count=kBindingsLeedarsonCTLightCount,
};

}  // namespace zhc::devices::leedarson
