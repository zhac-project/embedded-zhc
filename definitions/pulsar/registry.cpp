// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/pulsar_registry.hpp"

namespace zhc::devices::pulsar {

extern const PreparedDefinition kDef_LPC_V1;

const PreparedDefinition* const kPulsarRegistry[] = {
    &kDef_LPC_V1,
};

const std::size_t kPulsarRegistryCount = sizeof(kPulsarRegistry) / sizeof(kPulsarRegistry[0]);

}  // namespace zhc::devices::pulsar
