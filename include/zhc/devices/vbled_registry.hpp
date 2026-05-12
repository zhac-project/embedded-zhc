// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::vbled {

extern const PreparedDefinition* const kVbledRegistry[];
extern const std::size_t              kVbledRegistryCount;

}  // namespace zhc::devices::vbled
