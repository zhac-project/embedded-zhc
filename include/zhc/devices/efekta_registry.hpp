// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Efekta device registry. 69 devices; all use
// `m.*` modernExtend helpers over standard ZCL clusters, so
// stubs wire the shared `_generic` converters.

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::efekta {

extern const PreparedDefinition* const kEfektaRegistry[];
extern const std::size_t              kEfektaRegistryCount;

}  // namespace zhc::devices::efekta
