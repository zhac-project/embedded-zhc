// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated skydance registry.
#include "zhc/devices/skydance_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::skydance {

extern const PreparedDefinition kDef_WZ5_cct;
extern const PreparedDefinition kDef_WZ5_dim_1;
extern const PreparedDefinition kDef_WZ5_rgb;
extern const PreparedDefinition kDef_WZ5_rgbcct;
extern const PreparedDefinition kDef_WZ5_rgbw;
extern const PreparedDefinition kDef_WZ5_rgb_1;

const PreparedDefinition* const kSkydanceRegistry[] = {
    &kDef_WZ5_cct,
    &kDef_WZ5_dim_1,
    &kDef_WZ5_rgb,
    &kDef_WZ5_rgbcct,
    &kDef_WZ5_rgbw,
    &kDef_WZ5_rgb_1,
};
const std::size_t kSkydanceRegistryCount = sizeof(kSkydanceRegistry) / sizeof(kSkydanceRegistry[0]);

}  // namespace zhc::devices::skydance
