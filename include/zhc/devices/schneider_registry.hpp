// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Schneider Electric (Wiser / Clipsal / Merten) registry.
// 76 devices from schneider_electric.ts. Entries use mixed modernExtend
// + local fz/tz converters; stubs wire generic on-off / brightness /
// battery where detected.

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::schneider {

extern const PreparedDefinition* const kSchneiderRegistry[];
extern const std::size_t              kSchneiderRegistryCount;

}  // namespace zhc::devices::schneider
