// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated salus_controls registry.
#include "zhc/devices/salus_controls_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::salus_controls {

extern const PreparedDefinition kDef_FC600;
extern const PreparedDefinition kDef_SP600;
extern const PreparedDefinition kDef_SPE600;
extern const PreparedDefinition kDef_SR600;
extern const PreparedDefinition kDef_SX885ZB;
extern const PreparedDefinition kDef_OS600;
extern const PreparedDefinition kDef_PS600;
extern const PreparedDefinition kDef_SW600;
extern const PreparedDefinition kDef_WLS600;
extern const PreparedDefinition kDef_RE600;

const PreparedDefinition* const kSalusControlsRegistry[] = {
    &kDef_FC600,
    &kDef_SP600,
    &kDef_SPE600,
    &kDef_SR600,
    &kDef_SX885ZB,
    &kDef_OS600,
    &kDef_PS600,
    &kDef_SW600,
    &kDef_WLS600,
    &kDef_RE600,
};
const std::size_t kSalusControlsRegistryCount = sizeof(kSalusControlsRegistry) / sizeof(kSalusControlsRegistry[0]);

}  // namespace zhc::devices::salus_controls
