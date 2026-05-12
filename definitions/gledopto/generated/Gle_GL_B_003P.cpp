// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-B-003P — auto-generated.
// Zigbee 7W E26/E27 Bulb RGB+CCT (pro)
// z2m-source: gledopto.ts #GL-B-003P.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_B_003P[] = { "GL-B-003P" };
}  // namespace

extern const PreparedDefinition kDef_GL_B_003P{
    .zigbee_models=kModels_GL_B_003P,
    .zigbee_models_count=sizeof(kModels_GL_B_003P)/sizeof(kModels_GL_B_003P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-B-003P", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoCTLight, .exposes_count=kExposesGledoptoCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGledoptoCTLight, .from_zigbee_count=kFzGledoptoCTLightCount,
    .to_zigbee=kTzGledoptoCTLight,   .to_zigbee_count=kTzGledoptoCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoCTLight, .bindings_count=kBindingsGledoptoCTLightCount,
};

}  // namespace zhc::devices::gledopto
