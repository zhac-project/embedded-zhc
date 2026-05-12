// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Tuya device registry.

#include <cstddef>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tuya {

extern const PreparedDefinition* const kTuyaRegistry[];
extern const std::size_t               kTuyaRegistryCount;

}  // namespace zhc::devices::tuya
