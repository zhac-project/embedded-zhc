// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Aggregated Sonoff / Itead / eWeLink device registry. Most of z2m's
// sonoff.ts entries use the generic modernExtend helpers — we lower
// `m.onOff/battery/light/commandsOnOff` into stock `_generic`
// converters and leave per-device `m.numeric/binary/enumLookup` specs
// unported in v1 (devices still match by zigbeeModel and decode the
// common attrs).

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::sonoff {

extern const PreparedDefinition* const kSonoffRegistry[];
extern const std::size_t              kSonoffRegistryCount;

}  // namespace zhc::devices::sonoff
