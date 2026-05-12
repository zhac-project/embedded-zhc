// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Saswell device registry. Saswell's single z2m entry
// covers 20+ Tuya OEM thermostat fingerprints. v1 implements only
// fingerprint-match (legacy Tuya weekly-schedule converters remain
// unported — see PARITY_PLAN §T2).

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::saswell {

extern const PreparedDefinition* const kSaswellRegistry[];
extern const std::size_t              kSaswellRegistryCount;

}  // namespace zhc::devices::saswell
