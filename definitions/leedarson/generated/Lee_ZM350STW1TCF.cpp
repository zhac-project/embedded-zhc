// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson ZM350STW1TCF - hand-rewritten 2026-04-29.
// LED PAR16 50 GU10 tunable white
// z2m-source: leedarson.ts #ZM350STW1TCF.
// Mapping: m.light({colorTemp: {range: undefined, startup: false}})
//          -> kFz/kTz/kExposes/kBindings LeedarsonCTLight (OnOff+Brightness+ColorTemp).
#include "definitions/_generic/_shared.hpp"
#include "definitions/leedarson/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
constexpr const char* kModels_ZM350STW1TCF[] = { "LED_GU10_OWDT" };
}  // namespace

extern const PreparedDefinition kDef_ZM350STW1TCF{
    .zigbee_models=kModels_ZM350STW1TCF, .zigbee_models_count=sizeof(kModels_ZM350STW1TCF)/sizeof(kModels_ZM350STW1TCF[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZM350STW1TCF", .vendor="Leedarson",
    .meta=nullptr,
    .exposes=kExposesLeedarsonCTLight, .exposes_count=kExposesLeedarsonCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLeedarsonCTLight, .from_zigbee_count=kFzLeedarsonCTLightCount,
    .to_zigbee=kTzLeedarsonCTLight, .to_zigbee_count=kTzLeedarsonCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsLeedarsonCTLight, .bindings_count=kBindingsLeedarsonCTLightCount,
};

}  // namespace zhc::devices::leedarson
