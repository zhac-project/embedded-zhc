// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-C-006S — auto-generated.
// Zigbee LED Controller WW/CW (plus)
// z2m-source: gledopto.ts #GL-C-006S.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_C_006S[] = { "GL-C-006S" };
}  // namespace

extern const PreparedDefinition kDef_GL_C_006S{
    .zigbee_models=kModels_GL_C_006S,
    .zigbee_models_count=sizeof(kModels_GL_C_006S)/sizeof(kModels_GL_C_006S[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-C-006S", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoCTLight, .exposes_count=kExposesGledoptoCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGledoptoCTLight, .from_zigbee_count=kFzGledoptoCTLightCount,
    .to_zigbee=kTzGledoptoCTLight,   .to_zigbee_count=kTzGledoptoCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoCTLight, .bindings_count=kBindingsGledoptoCTLightCount,
};

}  // namespace zhc::devices::gledopto
