// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Moes device registry. Points to every ported Moes
// PreparedDefinition in `definitions/moes/`. Adapters iterate this
// array to resolve an inbound frame's model/manufacturer pair.
//
// Moes is a Tuya OEM — the devices here share the `manuSpecificTuya`
// (0xEF00) cluster and are discriminated by `_TZ*_xxx` manufacturer
// names. Matching goes through the same `find_definition` resolver
// that the Tuya registry uses.

#include <cstddef>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::moes {

extern const PreparedDefinition* const kMoesRegistry[];
extern const std::size_t               kMoesRegistryCount;

}  // namespace zhc::devices::moes
