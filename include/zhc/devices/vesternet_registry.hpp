// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::vesternet {

extern const PreparedDefinition* const kVesternetRegistry[];
extern const std::size_t              kVesternetRegistryCount;

}  // namespace zhc::devices::vesternet
