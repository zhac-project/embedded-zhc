// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-SD-301P — auto-generated.
// Zigbee triac AC dimmer
// z2m-source: gledopto.ts #GL-SD-301P.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_SD_301P[] = { "GL-SD-301P" };
}  // namespace

extern const PreparedDefinition kDef_GL_SD_301P{
    .zigbee_models=kModels_GL_SD_301P,
    .zigbee_models_count=sizeof(kModels_GL_SD_301P)/sizeof(kModels_GL_SD_301P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-SD-301P", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoLight, .exposes_count=kExposesGledoptoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGledoptoLight, .from_zigbee_count=kFzGledoptoLightCount,
    .to_zigbee=kTzGledoptoLight,   .to_zigbee_count=kTzGledoptoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoLight, .bindings_count=kBindingsGledoptoLightCount,
};

}  // namespace zhc::devices::gledopto
