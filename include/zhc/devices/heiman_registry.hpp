// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Heiman device registry. Heiman's 62 z2m entries mix
// standard ZCL on/off / battery / temperature / humidity / occupancy
// / IAS with per-device custom clusters. We lower the common-case
// modernExtend helpers into stock `_generic` converters; bespoke
// numeric/binary specs remain fingerprint-match only in v1.

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::heiman {

extern const PreparedDefinition* const kHeimanRegistry[];
extern const std::size_t              kHeimanRegistryCount;

}  // namespace zhc::devices::heiman
