// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated trust registry.
#include "zhc/devices/trust_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::trust {

extern const PreparedDefinition kDef_ZCM_300;
extern const PreparedDefinition kDef_ZLED_2709;
extern const PreparedDefinition kDef_ZLED_RGB9;
extern const PreparedDefinition kDef_ZLED_TUNE9;
extern const PreparedDefinition kDef_ZYCT_202;
extern const PreparedDefinition kDef_ZCTS_808;
extern const PreparedDefinition kDef_ZPIR_8000;
extern const PreparedDefinition kDef_ZSDR_850;
extern const PreparedDefinition kDef_ZWLD_100;

const PreparedDefinition* const kTrustRegistry[] = {
    &kDef_ZCM_300,
    &kDef_ZLED_2709,
    &kDef_ZLED_RGB9,
    &kDef_ZLED_TUNE9,
    &kDef_ZYCT_202,
    &kDef_ZCTS_808,
    &kDef_ZPIR_8000,
    &kDef_ZSDR_850,
    &kDef_ZWLD_100,
};
const std::size_t kTrustRegistryCount = sizeof(kTrustRegistry) / sizeof(kTrustRegistry[0]);

}  // namespace zhc::devices::trust
