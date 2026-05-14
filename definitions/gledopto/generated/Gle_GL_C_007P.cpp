// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-C-007P — auto-generated.
// Zigbee LED Controller RGBW (pro)
// z2m-source: gledopto.ts #GL-C-007P.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_C_007P[] = { "GL-C-007P" };
}  // namespace


constexpr WhiteLabel kWhiteLabels_GL_C_007P[] = {
    {"Gledopto","GL-C-007P_mini"},
};
extern const PreparedDefinition kDef_GL_C_007P{
    .zigbee_models=kModels_GL_C_007P,
    .zigbee_models_count=sizeof(kModels_GL_C_007P)/sizeof(kModels_GL_C_007P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-C-007P", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoColorCTLight, .exposes_count=kExposesGledoptoColorCTLightCount,
    .white_labels=kWhiteLabels_GL_C_007P, .white_labels_count=sizeof(kWhiteLabels_GL_C_007P)/sizeof(kWhiteLabels_GL_C_007P[0]),
    .from_zigbee=kFzGledoptoColorCTLight, .from_zigbee_count=kFzGledoptoColorCTLightCount,
    .to_zigbee=kTzGledoptoColorCTLight,   .to_zigbee_count=kTzGledoptoColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoColorCTLight, .bindings_count=kBindingsGledoptoColorCTLightCount,
};

}  // namespace zhc::devices::gledopto
