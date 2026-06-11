// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Openlumi GWRJN5169 — graduated override.
// Lumi Router (JN5169). z2m defines this device with a single modernExtend:
// `m.deviceTemperature()` (genDeviceTempCfg 0x0002 currentTemperature). It is
// a mains-powered router with NO on/off and NO battery. The Tier-1 auto-port
// invented a phantom on/off (kFzOnOff + state expose + 0x0006 bind) and a
// phantom battery (kFzBattery + battery/voltage exposes + 0x0001 bind), and
// dropped device_temperature entirely. This override restores the real, sole
// surface: device_temperature via the generic kFzDeviceTemperature, bound to
// genDeviceTempCfg (0x0002), with no phantom channels.
//
// z2m-source: openlumi.ts #GWRJN5169 (extend: [m.deviceTemperature()]) +
//             converters/fromZigbee.ts fz.device_temperature.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::openlumi {
namespace {


constexpr const char* kModels_GWRJN5169[] = { "openlumi.gw_router.jn5169" };

}  // namespace


namespace {
const FzConverter* const kFz_GWRJN5169[] = {
    &::zhc::generic::kFzDeviceTemperature,
};
constexpr Expose kExp_GWRJN5169[] = {
    { "device_temperature", ExposeType::Numeric, ::zhc::Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_GWRJN5169[] = {
    { 1, 0x0002 },   // genDeviceTempCfg
};
}
extern const PreparedDefinition kDef_GWRJN5169{
    .zigbee_models=kModels_GWRJN5169, .zigbee_models_count=sizeof(kModels_GWRJN5169)/sizeof(kModels_GWRJN5169[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GWRJN5169", .vendor="Openlumi",
    .meta=nullptr, .exposes=kExp_GWRJN5169, .exposes_count=sizeof(kExp_GWRJN5169)/sizeof(kExp_GWRJN5169[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_GWRJN5169, .from_zigbee_count=sizeof(kFz_GWRJN5169)/sizeof(kFz_GWRJN5169[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_GWRJN5169,
    .bindings_count           = sizeof(kBind_GWRJN5169)/sizeof(kBind_GWRJN5169[0]),
};

}  // namespace zhc::devices::openlumi
