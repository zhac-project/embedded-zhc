// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::custom_devices_diy {

extern const PreparedDefinition* const kCustomDevicesDiyRegistry[];
extern const std::size_t              kCustomDevicesDiyRegistryCount;

}  // namespace zhc::devices::custom_devices_diy
