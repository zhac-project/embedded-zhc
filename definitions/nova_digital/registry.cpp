// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/nova_digital_registry.hpp"

namespace zhc::devices::nova_digital {

extern const PreparedDefinition kDef_NFZB_2;
extern const PreparedDefinition kDef_ZCMR_1;
extern const PreparedDefinition kDef_ZTS_8B;
extern const PreparedDefinition kDef_SA_WK_4X4;
extern const PreparedDefinition kDef_TS0601_ND_4Gang;

const PreparedDefinition* const kNovaDigitalRegistry[] = {
    &kDef_NFZB_2,
    &kDef_ZCMR_1,
    &kDef_ZTS_8B,
    &kDef_SA_WK_4X4,
    &kDef_TS0601_ND_4Gang,
};

const std::size_t kNovaDigitalRegistryCount = sizeof(kNovaDigitalRegistry) / sizeof(kNovaDigitalRegistry[0]);

}  // namespace zhc::devices::nova_digital
