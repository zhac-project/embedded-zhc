// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated qmotion registry.
#include "zhc/devices/qmotion_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::qmotion {

extern const PreparedDefinition kDef_HDM40PV620;
extern const PreparedDefinition kDef_QZR_ZIG2400;

const PreparedDefinition* const kQmotionRegistry[] = {
    &kDef_HDM40PV620,
    &kDef_QZR_ZIG2400,
};
const std::size_t kQmotionRegistryCount = sizeof(kQmotionRegistry) / sizeof(kQmotionRegistry[0]);

}  // namespace zhc::devices::qmotion
