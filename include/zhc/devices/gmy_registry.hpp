// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::gmy {

extern const PreparedDefinition* const kGmyRegistry[];
extern const std::size_t              kGmyRegistryCount;

}  // namespace zhc::devices::gmy
