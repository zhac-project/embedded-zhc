// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated onesti registry.
#include "zhc/devices/onesti_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::onesti {

extern const PreparedDefinition kDef_Nimly;
extern const PreparedDefinition kDef_S4RX_110;
extern const PreparedDefinition kDef_easyCodeTouch_v1;

const PreparedDefinition* const kOnestiRegistry[] = {
    &kDef_Nimly,
    &kDef_S4RX_110,
    &kDef_easyCodeTouch_v1,
};
const std::size_t kOnestiRegistryCount = sizeof(kOnestiRegistry) / sizeof(kOnestiRegistry[0]);

}  // namespace zhc::devices::onesti
