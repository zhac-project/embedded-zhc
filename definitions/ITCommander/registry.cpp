// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated ITCommander registry.
#include "zhc/devices/ITCommander_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ITCommander {

extern const PreparedDefinition kDef_ITCMDR_Click;
extern const PreparedDefinition kDef_ITCMDR_Contact;

const PreparedDefinition* const kItcommanderRegistry[] = {
    &kDef_ITCMDR_Click,
    &kDef_ITCMDR_Contact,
};
const std::size_t kItcommanderRegistryCount = sizeof(kItcommanderRegistry) / sizeof(kItcommanderRegistry[0]);

}  // namespace zhc::devices::ITCommander
