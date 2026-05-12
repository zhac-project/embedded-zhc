// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated airam registry.
#include "zhc/devices/airam_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::airam {

extern const PreparedDefinition kDef_AIRAM_CTR_U;
extern const PreparedDefinition kDef_CTR_UBX;
extern const PreparedDefinition kDef_D4713406;
extern const PreparedDefinition kDef_D4713407;

const PreparedDefinition* const kAiramRegistry[] = {
    &kDef_AIRAM_CTR_U,
    &kDef_CTR_UBX,
    &kDef_D4713406,
    &kDef_D4713407,
};
const std::size_t kAiramRegistryCount = sizeof(kAiramRegistry) / sizeof(kAiramRegistry[0]);

}  // namespace zhc::devices::airam
