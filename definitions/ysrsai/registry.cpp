// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ysrsai registry.
#include "zhc/devices/ysrsai_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ysrsai {

extern const PreparedDefinition kDef_YSR_MINI_01_dimmer;
extern const PreparedDefinition kDef_YSR_MINI_01_rgbcct;
extern const PreparedDefinition kDef_YSR_MINI_01_wwcw;

const PreparedDefinition* const kYsrsaiRegistry[] = {
    &kDef_YSR_MINI_01_dimmer,
    &kDef_YSR_MINI_01_rgbcct,
    &kDef_YSR_MINI_01_wwcw,
};
const std::size_t kYsrsaiRegistryCount = sizeof(kYsrsaiRegistry) / sizeof(kYsrsaiRegistry[0]);

}  // namespace zhc::devices::ysrsai
