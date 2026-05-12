// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::viessmann {

extern const PreparedDefinition* const kViessmannRegistry[];
extern const std::size_t              kViessmannRegistryCount;

}  // namespace zhc::devices::viessmann
