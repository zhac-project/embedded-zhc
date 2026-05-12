// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::gewiss {

extern const PreparedDefinition* const kGewissRegistry[];
extern const std::size_t              kGewissRegistryCount;

}  // namespace zhc::devices::gewiss
