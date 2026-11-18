// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/devices/control4_registry.hpp"

namespace zhc::devices::control4 {

extern const PreparedDefinition kDef_VB_FTW_A19;
extern const PreparedDefinition kDef_VB_RGBTW_A19;

const PreparedDefinition* const kControl4Registry[] = {
    &kDef_VB_FTW_A19,
    &kDef_VB_RGBTW_A19,
};

const std::size_t kControl4RegistryCount = sizeof(kControl4Registry) / sizeof(kControl4Registry[0]);

}  // namespace zhc::devices::control4
