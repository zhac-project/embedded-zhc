// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::bankamp {

extern const PreparedDefinition* const kBankampRegistry[];
extern const std::size_t              kBankampRegistryCount;

}  // namespace zhc::devices::bankamp
