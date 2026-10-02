// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// ekaza registry.
#include "zhc/devices/ekaza_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ekaza {

extern const PreparedDefinition kDef_TS0225_EKAZA;

const PreparedDefinition* const kEkazaRegistry[] = {
    &kDef_TS0225_EKAZA,
};
const std::size_t kEkazaRegistryCount = sizeof(kEkazaRegistry) / sizeof(kEkazaRegistry[0]);

}  // namespace zhc::devices::ekaza
