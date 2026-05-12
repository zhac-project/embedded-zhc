// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated evn registry.
#include "zhc/devices/evn_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::evn {

extern const PreparedDefinition kDef_ZB24100VS;
extern const PreparedDefinition kDef_ZBHS4RGBW;
extern const PreparedDefinition kDef_ZBPD23400;

const PreparedDefinition* const kEvnRegistry[] = {
    &kDef_ZB24100VS,
    &kDef_ZBHS4RGBW,
    &kDef_ZBPD23400,
};
const std::size_t kEvnRegistryCount = sizeof(kEvnRegistry) / sizeof(kEvnRegistry[0]);

}  // namespace zhc::devices::evn
