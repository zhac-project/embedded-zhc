// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated linptech registry.
#include "zhc/devices/linptech_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::linptech {

extern const PreparedDefinition kDefLin__TZ3218_awarhusb;
extern const PreparedDefinition kDefLin__TZ3218_ewrxirng;
extern const PreparedDefinition kDefLin__TZ3218_t9ynfz4x;
extern const PreparedDefinition kDef_ES1ZZ_TY_;

const PreparedDefinition* const kLinptechRegistry[] = {
    &kDefLin__TZ3218_awarhusb,
    &kDefLin__TZ3218_ewrxirng,
    &kDefLin__TZ3218_t9ynfz4x,
    &kDef_ES1ZZ_TY_,
};
const std::size_t kLinptechRegistryCount = sizeof(kLinptechRegistry) / sizeof(kLinptechRegistry[0]);

}  // namespace zhc::devices::linptech
