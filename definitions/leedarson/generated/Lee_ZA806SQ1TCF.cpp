// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson ZA806SQ1TCF - hand-rewritten 2026-04-29.
// LED E27 tunable white
// z2m-source: leedarson.ts #ZA806SQ1TCF.
// Mapping: m.light({colorTemp: {range: undefined}})
//          -> kFz/kTz/kExposes/kBindings LeedarsonCTLight (OnOff+Brightness+ColorTemp).
#include "definitions/_generic/_shared.hpp"
#include "definitions/leedarson/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
constexpr const char* kModels_ZA806SQ1TCF[] = { "LED_E27_OWDT" };
}  // namespace

extern const PreparedDefinition kDef_ZA806SQ1TCF{
    .zigbee_models=kModels_ZA806SQ1TCF, .zigbee_models_count=sizeof(kModels_ZA806SQ1TCF)/sizeof(kModels_ZA806SQ1TCF[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZA806SQ1TCF", .vendor="Leedarson",
    .meta=nullptr,
    .exposes=kExposesLeedarsonCTLight, .exposes_count=kExposesLeedarsonCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLeedarsonCTLight, .from_zigbee_count=kFzLeedarsonCTLightCount,
    .to_zigbee=kTzLeedarsonCTLight, .to_zigbee_count=kTzLeedarsonCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsLeedarsonCTLight, .bindings_count=kBindingsLeedarsonCTLightCount,
};

}  // namespace zhc::devices::leedarson
