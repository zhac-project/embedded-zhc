// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated tplink registry.
#include "zhc/devices/tplink_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tplink {

extern const PreparedDefinition kDef_CS100;
extern const PreparedDefinition kDef_MS100;

const PreparedDefinition* const kTplinkRegistry[] = {
    &kDef_CS100,
    &kDef_MS100,
};
const std::size_t kTplinkRegistryCount = sizeof(kTplinkRegistry) / sizeof(kTplinkRegistry[0]);

}  // namespace zhc::devices::tplink
