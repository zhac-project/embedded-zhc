// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy CC2538.ROUTER.V2 — auto-generated.
// MODKAM stick CC2538 router with temperature sensor
// z2m-source: custom_devices_diy.ts #CC2538.ROUTER.V2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: fromZigbee=[fz.device_temperature], toZigbee=[], exposes=[e.device_temperature()].
// Router with on-board temp sensor (genDeviceTemperatureCfg cluster 0x0002 attr 0x0000).
// TODO(parity): no kFzDeviceTemperature helper in shared (cluster 0x0002 attr 0x0000 s16
// in 1 °C steps). Add a generic device_temperature decoder to _generic/_shared.
constexpr const char* kModels_CC2538_ROUTER_V2[] = { "cc2538.router.v2" };
}  // namespace
extern const PreparedDefinition kDef_CC2538_ROUTER_V2{
    .zigbee_models=kModels_CC2538_ROUTER_V2, .zigbee_models_count=sizeof(kModels_CC2538_ROUTER_V2)/sizeof(kModels_CC2538_ROUTER_V2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CC2538.ROUTER.V2", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::custom_devices_diy
