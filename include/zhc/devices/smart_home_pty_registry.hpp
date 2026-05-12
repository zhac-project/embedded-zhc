// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::smart_home_pty {

extern const PreparedDefinition* const kSmartHomePtyRegistry[];
extern const std::size_t              kSmartHomePtyRegistryCount;

}  // namespace zhc::devices::smart_home_pty
