// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GL-C-009P — auto-generated.
// Zigbee LED Controller W (pro)
// z2m-source: gledopto.ts #GL-C-009P.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GL_C_009P[] = { "GL-C-009P" };
}  // namespace


constexpr WhiteLabel kWhiteLabels_GL_C_009P[] = {
    {"Gledopto","GL-C-009P_mini"},
};
extern const PreparedDefinition kDef_GL_C_009P{
    .zigbee_models=kModels_GL_C_009P,
    .zigbee_models_count=sizeof(kModels_GL_C_009P)/sizeof(kModels_GL_C_009P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GL-C-009P", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoLight, .exposes_count=kExposesGledoptoLightCount,
    .white_labels=kWhiteLabels_GL_C_009P, .white_labels_count=sizeof(kWhiteLabels_GL_C_009P)/sizeof(kWhiteLabels_GL_C_009P[0]),
    .from_zigbee=kFzGledoptoLight, .from_zigbee_count=kFzGledoptoLightCount,
    .to_zigbee=kTzGledoptoLight,   .to_zigbee_count=kTzGledoptoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoLight, .bindings_count=kBindingsGledoptoLightCount,
};

}  // namespace zhc::devices::gledopto
