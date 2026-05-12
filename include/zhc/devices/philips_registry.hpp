// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Philips Hue device registry. Auto-generated shells for
// every `philips.m.light(...)` / `philips.m.onOff(...)` entry in z2m's
// philips.ts. All devices use the stock `_generic` on-off / brightness
// / colour-temperature converters — the Hue range is mechanically
// uniform once you flatten the `extend` helpers.

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::philips {

extern const PreparedDefinition* const kPhilipsRegistry[];
extern const std::size_t              kPhilipsRegistryCount;

}  // namespace zhc::devices::philips
