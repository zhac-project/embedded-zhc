// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson 6xy-M350ST-W1Z - hand-rewritten 2026-04-29.
// PAR16 tunable white
// z2m-source: leedarson.ts #6xy-M350ST-W1Z.
// Mapping: m.light({colorTemp: {range: undefined}})
//          -> kFz/kTz/kExposes/kBindings LeedarsonCTLight (OnOff+Brightness+ColorTemp).
#include "definitions/_generic/_shared.hpp"
#include "definitions/leedarson/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
constexpr const char* kModels_D6xy_M350ST_W1Z[] = { "TWGU10Bulb02UK" };
}  // namespace

extern const PreparedDefinition kDef_D6xy_M350ST_W1Z{
    .zigbee_models=kModels_D6xy_M350ST_W1Z, .zigbee_models_count=sizeof(kModels_D6xy_M350ST_W1Z)/sizeof(kModels_D6xy_M350ST_W1Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="6xy-M350ST-W1Z", .vendor="Leedarson",
    .meta=nullptr,
    .exposes=kExposesLeedarsonCTLight, .exposes_count=kExposesLeedarsonCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLeedarsonCTLight, .from_zigbee_count=kFzLeedarsonCTLightCount,
    .to_zigbee=kTzLeedarsonCTLight, .to_zigbee_count=kTzLeedarsonCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsLeedarsonCTLight, .bindings_count=kBindingsLeedarsonCTLightCount,
};

}  // namespace zhc::devices::leedarson
