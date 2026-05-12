// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy CC2530.ROUTER — auto-generated.
// CC2530 router
// z2m-source: custom_devices_diy.ts #CC2530.ROUTER.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: fromZigbee=[fz.CC2530ROUTER_led, fz.CC2530ROUTER_meta], toZigbee=[tz.ptvo_switch_trigger],
// exposes=[e.binary("led", STATE, true, false)]. Router-only DIY firmware on TI CC2530;
// "led" reflects the on-board indicator LED, no on/off relay.
// TODO(parity): no shared helper for fz.CC2530ROUTER_led/meta (manuf-specific 0xFC00 attrs)
// or tz.ptvo_switch_trigger. State exposed currently as standard genOnOff for visibility.
constexpr const char* kModels_CC2530_ROUTER[] = { "lumi.router" };

const FzConverter* const kFz_CC2530_ROUTER[] = {
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_CC2530_ROUTER[] = {
    { "led", ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_CC2530_ROUTER[] = {
    { 1, 0x0006 },
};
}  // namespace
extern const PreparedDefinition kDef_CC2530_ROUTER{
    .zigbee_models=kModels_CC2530_ROUTER, .zigbee_models_count=sizeof(kModels_CC2530_ROUTER)/sizeof(kModels_CC2530_ROUTER[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CC2530.ROUTER", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kExp_CC2530_ROUTER, .exposes_count=sizeof(kExp_CC2530_ROUTER)/sizeof(kExp_CC2530_ROUTER[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CC2530_ROUTER, .from_zigbee_count=sizeof(kFz_CC2530_ROUTER)/sizeof(kFz_CC2530_ROUTER[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_CC2530_ROUTER,
    .bindings_count           = sizeof(kBind_CC2530_ROUTER)/sizeof(kBind_CC2530_ROUTER[0]),
};

}  // namespace zhc::devices::custom_devices_diy
