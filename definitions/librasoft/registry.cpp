// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/librasoft_registry.hpp"

namespace zhc::devices::librasoft {

extern const PreparedDefinition kDef_visspoint_io4;

const PreparedDefinition* const kLibrasoftRegistry[] = {
    &kDef_visspoint_io4,
};

const std::size_t kLibrasoftRegistryCount = sizeof(kLibrasoftRegistry) / sizeof(kLibrasoftRegistry[0]);

}  // namespace zhc::devices::librasoft
