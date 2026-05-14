// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-C-008P — auto-generated.
// Zigbee LED Controller RGB+CCT (pro)
// z2m-source: gledopto.ts #GL-C-008P.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_C_008P[] = { "GL-C-008P", "C-ZB-LC20v2-RGBCCT" };
}  // namespace


constexpr WhiteLabel kWhiteLabels_GL_C_008P[] = {
    {"Gledopto","GL-C-008P_mini"},
    {"Gledopto","GL-C-001P"},
    {"Gledopto","GL-C-002P"},
    {"Gledopto","GL-C-011P"},
    {"Gledopto","GL-C-201P"},
    {"Gledopto","GL-C-202P"},
    {"Gledopto","GL-C-204P"},
    {"Gledopto","GL-C-301P"},
};
extern const PreparedDefinition kDef_GL_C_008P{
    .zigbee_models=kModels_GL_C_008P,
    .zigbee_models_count=sizeof(kModels_GL_C_008P)/sizeof(kModels_GL_C_008P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-C-008P", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoColorCTLight, .exposes_count=kExposesGledoptoColorCTLightCount,
    .white_labels=kWhiteLabels_GL_C_008P, .white_labels_count=sizeof(kWhiteLabels_GL_C_008P)/sizeof(kWhiteLabels_GL_C_008P[0]),
    .from_zigbee=kFzGledoptoColorCTLight, .from_zigbee_count=kFzGledoptoColorCTLightCount,
    .to_zigbee=kTzGledoptoColorCTLight,   .to_zigbee_count=kTzGledoptoColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoColorCTLight, .bindings_count=kBindingsGledoptoColorCTLightCount,
};

}  // namespace zhc::devices::gledopto
