// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated aeotec registry.
#include "zhc/devices/aeotec_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::aeotec {

extern const PreparedDefinition kDef_WG001;
extern const PreparedDefinition kDef_ZGA002;
extern const PreparedDefinition kDef_ZGA003;
extern const PreparedDefinition kDef_ZGA004;

const PreparedDefinition* const kAeotecRegistry[] = {
    &kDef_WG001,
    &kDef_ZGA002,
    &kDef_ZGA003,
    &kDef_ZGA004,
};
const std::size_t kAeotecRegistryCount = sizeof(kAeotecRegistry) / sizeof(kAeotecRegistry[0]);

}  // namespace zhc::devices::aeotec
