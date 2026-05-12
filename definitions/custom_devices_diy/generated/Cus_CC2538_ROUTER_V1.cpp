// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy CC2538.ROUTER.V1 — auto-generated.
// MODKAM stick CC2538 router
// z2m-source: custom_devices_diy.ts #CC2538.ROUTER.V1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: fromZigbee=[], toZigbee=[], exposes=[]. Router-only firmware on MODKAM CC2538.
// No reportable state in upstream definition (just an interview target for pairing).
constexpr const char* kModels_CC2538_ROUTER_V1[] = { "cc2538.router.v1" };
}  // namespace
extern const PreparedDefinition kDef_CC2538_ROUTER_V1{
    .zigbee_models=kModels_CC2538_ROUTER_V1, .zigbee_models_count=sizeof(kModels_CC2538_ROUTER_V1)/sizeof(kModels_CC2538_ROUTER_V1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CC2538.ROUTER.V1", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::custom_devices_diy
