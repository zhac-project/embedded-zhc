// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy ti.router — auto-generated.
// Texas Instruments router
// z2m-source: custom_devices_diy.ts #ti.router.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: fromZigbee=[fzLocal.tirouter (genBasic attr 0x1337 = transmit_power)],
// toZigbee=[tzLocal.tirouter (writes attr 0x1337 type 0x28)], exposes=[transmit_power numeric].
// Router-only DIY firmware on CC1352/CC2652 (zigbee2mqtt sample router); no on/off, no battery.
// TODO(parity): no shared helper for non-standard genBasic 0x1337 attribute. Add custom
// fz/tz pair to expose transmit_power (-20..+20 dBm).
constexpr const char* kModels_ti_router[] = { "ti.router" };
}  // namespace
extern const PreparedDefinition kDef_ti_router{
    .zigbee_models=kModels_ti_router, .zigbee_models_count=sizeof(kModels_ti_router)/sizeof(kModels_ti_router[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ti.router", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::custom_devices_diy
