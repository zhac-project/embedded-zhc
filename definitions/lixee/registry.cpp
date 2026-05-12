// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated lixee registry.
#include "zhc/devices/lixee_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lixee {

extern const PreparedDefinition kDef_ZiPulses;
extern const PreparedDefinition kDef_ZLinky_TIC;

const PreparedDefinition* const kLixeeRegistry[] = {
    &kDef_ZiPulses,
    &kDef_ZLinky_TIC,
};
const std::size_t kLixeeRegistryCount = sizeof(kLixeeRegistry) / sizeof(kLixeeRegistry[0]);

}  // namespace zhc::devices::lixee
