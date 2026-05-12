// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::legrand {

extern const PreparedDefinition* const kLegrandRegistry[];
extern const std::size_t              kLegrandRegistryCount;

}  // namespace zhc::devices::legrand
