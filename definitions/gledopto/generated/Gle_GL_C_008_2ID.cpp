// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-C-008-2ID — auto-generated.
// Zigbee LED Controller RGB+CCT (2 ID)
// z2m-source: gledopto.ts #GL-C-008-2ID.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_C_008_2ID[] = { "GL-C-008-2ID" };
}  // namespace

extern const PreparedDefinition kDef_GL_C_008_2ID{
    .zigbee_models=kModels_GL_C_008_2ID,
    .zigbee_models_count=sizeof(kModels_GL_C_008_2ID)/sizeof(kModels_GL_C_008_2ID[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-C-008-2ID", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoColorCTLight, .exposes_count=kExposesGledoptoColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGledoptoColorCTLight, .from_zigbee_count=kFzGledoptoColorCTLightCount,
    .to_zigbee=kTzGledoptoColorCTLight,   .to_zigbee_count=kTzGledoptoColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoColorCTLight, .bindings_count=kBindingsGledoptoColorCTLightCount,
};

}  // namespace zhc::devices::gledopto
