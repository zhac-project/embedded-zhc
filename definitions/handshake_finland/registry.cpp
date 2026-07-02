// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// handshake_finland device registry.
#include "zhc/devices/handshake_finland_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::handshake_finland {

extern const PreparedDefinition kDef_2619839;

const PreparedDefinition* const kHandshakeFinlandRegistry[] = {
    &kDef_2619839,
};
const std::size_t kHandshakeFinlandRegistryCount = sizeof(kHandshakeFinlandRegistry) / sizeof(kHandshakeFinlandRegistry[0]);

}  // namespace zhc::devices::handshake_finland
