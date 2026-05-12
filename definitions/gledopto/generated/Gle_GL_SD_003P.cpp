// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-SD-003P — auto-generated.
// Zigbee DIN Rail triac AC dimmer
// z2m-source: gledopto.ts #GL-SD-003P.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_SD_003P[] = { "GL-SD-003P" };
}  // namespace

extern const PreparedDefinition kDef_GL_SD_003P{
    .zigbee_models=kModels_GL_SD_003P,
    .zigbee_models_count=sizeof(kModels_GL_SD_003P)/sizeof(kModels_GL_SD_003P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-SD-003P", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoLight, .exposes_count=kExposesGledoptoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGledoptoLight, .from_zigbee_count=kFzGledoptoLightCount,
    .to_zigbee=kTzGledoptoLight,   .to_zigbee_count=kTzGledoptoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoLight, .bindings_count=kBindingsGledoptoLightCount,
};

}  // namespace zhc::devices::gledopto
