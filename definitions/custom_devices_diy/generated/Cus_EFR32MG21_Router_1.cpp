// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy EFR32MG21.Router.1 — auto-generated.
// EFR32MG21 Zigbee bridge router
// z2m-source: custom_devices_diy.ts #EFR32MG21.Router.1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: fromZigbee=[], toZigbee=[], exposes=[]. Router-only device, nothing to expose.
constexpr const char* kModels_EFR32MG21_Router_1[] = { "UT-01" };
}  // namespace
extern const PreparedDefinition kDef_EFR32MG21_Router_1{
    .zigbee_models=kModels_EFR32MG21_Router_1, .zigbee_models_count=sizeof(kModels_EFR32MG21_Router_1)/sizeof(kModels_EFR32MG21_Router_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EFR32MG21.Router.1", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::custom_devices_diy
