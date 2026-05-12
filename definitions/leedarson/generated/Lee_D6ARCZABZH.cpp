// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1 (PARTIAL): Leedarson 6ARCZABZH — hand-rewritten 2026-04-29.
// 4-Key Remote Controller (battery-powered)
// z2m-source: leedarson.ts #6ARCZABZH.
// Mapping: fromZigbee=[fz.command_on, fz.command_off,
//                       fz.CCTSwitch_D0001_levelctrl,
//                       fz.CCTSwitch_D0001_lighting, fz.battery]
//          on/off -> kFzCommandOn / kFzCommandOff (action emission, NOT state).
//          battery -> kFzBattery.
//          PARTIAL: CCTSwitch_D0001_levelctrl / CCTSwitch_D0001_lighting are
//          Leedarson-specific raw level/colorTemp action decoders that emit
//          colortemp_up/_down/_release/_hold + brightness_up/_down. ZHC's
//          generic kFzCommandStep/Move/MoveColorTemperature partially overlap
//          but don't yield the exact "colortemp_up_release" labels the z2m
//          expose list advertises, so the Leedarson actions are limited to
//          the on/off pair until a CCTSwitch_D0001 typed converter lands.
//          The remote is exposed with `action` (Enum) so the SPA still
//          surfaces button press events for on/off.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
const FzConverter* const kFz_D6ARCZABZH[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D6ARCZABZH[] = { "ZBT-CCTSwitch-D0001" };

constexpr Expose kExposes_D6ARCZABZH[] = {
    { "battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
    { "action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D6ARCZABZH[] = {
    { 1, 0x0001 },  // genPowerCfg (battery)
    { 1, 0x0006 },  // genOnOff   (command_on / command_off actions)
    { 1, 0x0008 },  // genLevelCtrl (level commands; not yet decoded)
    { 1, 0x0300 },  // lightingColorCtrl (colorTemp commands; not yet decoded)
};
}  // namespace

extern const PreparedDefinition kDef_D6ARCZABZH{
    .zigbee_models=kModels_D6ARCZABZH, .zigbee_models_count=sizeof(kModels_D6ARCZABZH)/sizeof(kModels_D6ARCZABZH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="6ARCZABZH", .vendor="Leedarson",
    .meta=nullptr, .exposes=kExposes_D6ARCZABZH, .exposes_count=sizeof(kExposes_D6ARCZABZH)/sizeof(kExposes_D6ARCZABZH[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D6ARCZABZH, .from_zigbee_count=sizeof(kFz_D6ARCZABZH)/sizeof(kFz_D6ARCZABZH[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D6ARCZABZH, .bindings_count=sizeof(kBindings_D6ARCZABZH)/sizeof(kBindings_D6ARCZABZH[0]),
};

}  // namespace zhc::devices::leedarson
