// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated giex registry.
#include "zhc/devices/giex_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::giex {

extern const PreparedDefinition kDef_QT06_1;
extern const PreparedDefinition kDef_QT06_2;

const PreparedDefinition* const kGiexRegistry[] = {
    &kDef_QT06_1,
    &kDef_QT06_2,
};
const std::size_t kGiexRegistryCount = sizeof(kGiexRegistry) / sizeof(kGiexRegistry[0]);

}  // namespace zhc::devices::giex
