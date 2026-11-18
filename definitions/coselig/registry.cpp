// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/coselig_registry.hpp"

namespace zhc::devices::coselig {

extern const PreparedDefinition kDef_H2_ZB_SW_V1;

const PreparedDefinition* const kCoseligRegistry[] = {
    &kDef_H2_ZB_SW_V1,
};

const std::size_t kCoseligRegistryCount = sizeof(kCoseligRegistry) / sizeof(kCoseligRegistry[0]);

}  // namespace zhc::devices::coselig
