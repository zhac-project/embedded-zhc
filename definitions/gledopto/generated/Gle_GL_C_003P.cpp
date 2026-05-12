// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-C-003P — auto-generated.
// Zigbee LED Controller RGB (pro)
// z2m-source: gledopto.ts #GL-C-003P.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_C_003P[] = { "GL-C-003P" };
}  // namespace

extern const PreparedDefinition kDef_GL_C_003P{
    .zigbee_models=kModels_GL_C_003P,
    .zigbee_models_count=sizeof(kModels_GL_C_003P)/sizeof(kModels_GL_C_003P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-C-003P", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoColorLight, .exposes_count=kExposesGledoptoColorLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGledoptoColorLight, .from_zigbee_count=kFzGledoptoColorLightCount,
    .to_zigbee=kTzGledoptoColorLight,   .to_zigbee_count=kTzGledoptoColorLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoColorLight, .bindings_count=kBindingsGledoptoColorLightCount,
};

}  // namespace zhc::devices::gledopto
