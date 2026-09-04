// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// siemens registry.
#include "zhc/devices/siemens_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::siemens {

extern const PreparedDefinition kDef_RDZ101ZB;

const PreparedDefinition* const kSiemensRegistry[] = {
    &kDef_RDZ101ZB,
};
const std::size_t kSiemensRegistryCount = sizeof(kSiemensRegistry) / sizeof(kSiemensRegistry[0]);

}  // namespace zhc::devices::siemens
