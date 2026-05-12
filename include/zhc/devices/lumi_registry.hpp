// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Lumi device registry. Points to every ported Lumi
// PreparedDefinition in `definitions/lumi/*.cpp`. Adapters iterate
// this array (e.g. via `zhc::find_definition_by_model`) to resolve an
// inbound frame's `modelIdentifier` without hardcoding per-device
// extern declarations.

#include <cstddef>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {

extern const PreparedDefinition* const kLumiRegistry[];
extern const std::size_t               kLumiRegistryCount;

}  // namespace zhc::devices::lumi
