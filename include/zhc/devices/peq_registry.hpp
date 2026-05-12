// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::peq {

extern const PreparedDefinition* const kPeqRegistry[];
extern const std::size_t              kPeqRegistryCount;

}  // namespace zhc::devices::peq
