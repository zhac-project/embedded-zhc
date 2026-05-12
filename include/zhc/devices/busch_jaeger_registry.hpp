// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::busch_jaeger {

extern const PreparedDefinition* const kBuschJaegerRegistry[];
extern const std::size_t              kBuschJaegerRegistryCount;

}  // namespace zhc::devices::busch_jaeger
