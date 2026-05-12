// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::solaredge {

extern const PreparedDefinition* const kSolaredgeRegistry[];
extern const std::size_t              kSolaredgeRegistryCount;

}  // namespace zhc::devices::solaredge
