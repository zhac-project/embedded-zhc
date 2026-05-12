// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated vav registry.
#include "zhc/devices/vav_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::vav {

extern const PreparedDefinition kDef_VAV_256215_MOD1;
extern const PreparedDefinition kDef_VAV_256215_MOD2;

const PreparedDefinition* const kVavRegistry[] = {
    &kDef_VAV_256215_MOD1,
    &kDef_VAV_256215_MOD2,
};
const std::size_t kVavRegistryCount = sizeof(kVavRegistry) / sizeof(kVavRegistry[0]);

}  // namespace zhc::devices::vav
