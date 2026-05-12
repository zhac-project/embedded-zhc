// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated databyte registry.
#include "zhc/devices/databyte_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::databyte {

extern const PreparedDefinition kDef_ED2004_012;
extern const PreparedDefinition kDef_Touch4;
extern const PreparedDefinition kDef_DTB190502A1;

const PreparedDefinition* const kDatabyteRegistry[] = {
    &kDef_ED2004_012,
    &kDef_Touch4,
    &kDef_DTB190502A1,
};
const std::size_t kDatabyteRegistryCount = sizeof(kDatabyteRegistry) / sizeof(kDatabyteRegistry[0]);

}  // namespace zhc::devices::databyte
