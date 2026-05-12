// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated jiawen registry.
#include "zhc/devices/jiawen_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::jiawen {

extern const PreparedDefinition kDef_JW_A04_CT;
extern const PreparedDefinition kDef_K2RGBW01;

const PreparedDefinition* const kJiawenRegistry[] = {
    &kDef_JW_A04_CT,
    &kDef_K2RGBW01,
};
const std::size_t kJiawenRegistryCount = sizeof(kJiawenRegistry) / sizeof(kJiawenRegistry[0]);

}  // namespace zhc::devices::jiawen
