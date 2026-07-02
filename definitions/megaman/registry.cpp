// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Megaman device registry.
#include "zhc/devices/megaman_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::megaman {

extern const PreparedDefinition kDef_LC201060;

const PreparedDefinition* const kMegamanRegistry[] = {
    &kDef_LC201060,
};
const std::size_t kMegamanRegistryCount = sizeof(kMegamanRegistry) / sizeof(kMegamanRegistry[0]);

}  // namespace zhc::devices::megaman
