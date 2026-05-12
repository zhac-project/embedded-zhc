// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated livolo registry.
#include "zhc/devices/livolo_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::livolo {

extern const PreparedDefinition kDef_TI0001_hygrometer;
extern const PreparedDefinition kDef_TI0001_illuminance;
extern const PreparedDefinition kDef_TI0001;
extern const PreparedDefinition kDef_TI0001_cover;
extern const PreparedDefinition kDef_TI0001_curtain_switch;
extern const PreparedDefinition kDef_TI0001_dimmer;
extern const PreparedDefinition kDef_TI0001_socket;
extern const PreparedDefinition kDef_TI0001_switch;
extern const PreparedDefinition kDef_TI0001_switch_2gang;
extern const PreparedDefinition kDef_TI0001_pir;

const PreparedDefinition* const kLivoloRegistry[] = {
    &kDef_TI0001_hygrometer,
    &kDef_TI0001_illuminance,
    &kDef_TI0001,
    &kDef_TI0001_cover,
    &kDef_TI0001_curtain_switch,
    &kDef_TI0001_dimmer,
    &kDef_TI0001_socket,
    &kDef_TI0001_switch,
    &kDef_TI0001_switch_2gang,
    &kDef_TI0001_pir,
};
const std::size_t kLivoloRegistryCount = sizeof(kLivoloRegistry) / sizeof(kLivoloRegistry[0]);

}  // namespace zhc::devices::livolo
