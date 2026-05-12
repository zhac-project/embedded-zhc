// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated sercomm registry.
#include "zhc/devices/sercomm_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::sercomm {

extern const PreparedDefinition kDef_SZ_ESW01;
extern const PreparedDefinition kDef_SZ_ESW01_AU;
extern const PreparedDefinition kDef_SZ_ESW02;
extern const PreparedDefinition kDef_SZ_ESW02N_CZ3;
extern const PreparedDefinition kDef_AL_PIR02;
extern const PreparedDefinition kDef_SZ_DWS04;
extern const PreparedDefinition kDef_SZ_DWS08;
extern const PreparedDefinition kDef_SZ_PIR04N;
extern const PreparedDefinition kDef_SZ_WTD03;
extern const PreparedDefinition kDef_XHS2_SE;

const PreparedDefinition* const kSercommRegistry[] = {
    &kDef_SZ_ESW01,
    &kDef_SZ_ESW01_AU,
    &kDef_SZ_ESW02,
    &kDef_SZ_ESW02N_CZ3,
    &kDef_AL_PIR02,
    &kDef_SZ_DWS04,
    &kDef_SZ_DWS08,
    &kDef_SZ_PIR04N,
    &kDef_SZ_WTD03,
    &kDef_XHS2_SE,
};
const std::size_t kSercommRegistryCount = sizeof(kSercommRegistry) / sizeof(kSercommRegistry[0]);

}  // namespace zhc::devices::sercomm
