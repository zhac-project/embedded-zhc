// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::edp {

extern const PreparedDefinition* const kEdpRegistry[];
extern const std::size_t              kEdpRegistryCount;

}  // namespace zhc::devices::edp
