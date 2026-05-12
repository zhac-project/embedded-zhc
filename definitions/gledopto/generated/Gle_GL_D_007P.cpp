// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-D-007P — auto-generated.
// Zigbee 12W anti-glare downlight RGB+CCT (pro)
// z2m-source: gledopto.ts #GL-D-007P.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_D_007P[] = { "GL-D-007P" };
}  // namespace

extern const PreparedDefinition kDef_GL_D_007P{
    .zigbee_models=kModels_GL_D_007P,
    .zigbee_models_count=sizeof(kModels_GL_D_007P)/sizeof(kModels_GL_D_007P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-D-007P", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoColorCTLight, .exposes_count=kExposesGledoptoColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGledoptoColorCTLight, .from_zigbee_count=kFzGledoptoColorCTLightCount,
    .to_zigbee=kTzGledoptoColorCTLight,   .to_zigbee_count=kTzGledoptoColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoColorCTLight, .bindings_count=kBindingsGledoptoColorCTLightCount,
};

}  // namespace zhc::devices::gledopto
