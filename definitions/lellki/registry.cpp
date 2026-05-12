// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated lellki registry.
#include "zhc/devices/lellki_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lellki {

extern const PreparedDefinition kDef_CM001;
extern const PreparedDefinition kDef_JZ_ZB_001;
extern const PreparedDefinition kDef_JZ_ZB_002;
extern const PreparedDefinition kDef_JZ_ZB_003;
extern const PreparedDefinition kDef_WK34_EU;
extern const PreparedDefinition kDef_WP30_EU;
extern const PreparedDefinition kDef_WP33_EU_WP34_EU;
extern const PreparedDefinition kDef_XF_EU_S100_1_M;

const PreparedDefinition* const kLellkiRegistry[] = {
    &kDef_CM001,
    &kDef_JZ_ZB_001,
    &kDef_JZ_ZB_002,
    &kDef_JZ_ZB_003,
    &kDef_WK34_EU,
    &kDef_WP30_EU,
    &kDef_WP33_EU_WP34_EU,
    &kDef_XF_EU_S100_1_M,
};
const std::size_t kLellkiRegistryCount = sizeof(kLellkiRegistry) / sizeof(kLellkiRegistry[0]);

}  // namespace zhc::devices::lellki
