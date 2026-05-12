// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated smart_home_pty registry.
#include "zhc/devices/smart_home_pty_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::smart_home_pty {

extern const PreparedDefinition kDef_HGZB_07A;
extern const PreparedDefinition kDef_HGZB_20_DE;

const PreparedDefinition* const kSmartHomePtyRegistry[] = {
    &kDef_HGZB_07A,
    &kDef_HGZB_20_DE,
};
const std::size_t kSmartHomePtyRegistryCount = sizeof(kSmartHomePtyRegistry) / sizeof(kSmartHomePtyRegistry[0]);

}  // namespace zhc::devices::smart_home_pty
