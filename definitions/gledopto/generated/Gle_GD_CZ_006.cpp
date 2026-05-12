// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Gledopto GD-CZ-006 — auto-generated.
// Zigbee LED Controller WW/CW
// z2m-source: gledopto.ts #GD-CZ-006.
#include "definitions/_generic/_shared.hpp"
#include "definitions/gledopto/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
constexpr const char* kModels_GD_CZ_006[] = { "HOMA2023" };
}  // namespace

extern const PreparedDefinition kDef_GD_CZ_006{
    .zigbee_models=kModels_GD_CZ_006,
    .zigbee_models_count=sizeof(kModels_GD_CZ_006)/sizeof(kModels_GD_CZ_006[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GD-CZ-006", .vendor="Gledopto",
    .meta=nullptr,
    .exposes=kExposesGledoptoLight, .exposes_count=kExposesGledoptoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGledoptoLight, .from_zigbee_count=kFzGledoptoLightCount,
    .to_zigbee=kTzGledoptoLight,   .to_zigbee_count=kTzGledoptoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsGledoptoLight, .bindings_count=kBindingsGledoptoLightCount,
};

}  // namespace zhc::devices::gledopto
