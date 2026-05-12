// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::javis {

extern const PreparedDefinition* const kJavisRegistry[];
extern const std::size_t              kJavisRegistryCount;

}  // namespace zhc::devices::javis
