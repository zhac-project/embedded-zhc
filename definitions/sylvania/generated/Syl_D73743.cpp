// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sylvania 73743 — hand-port.
// Lightify Smart Dimming Switch — battery-powered scene remote.
// z2m-source: sylvania.ts #73743.
//
// z2m wiring: fz.command_on / fz.command_off / fz.command_move /
// fz.command_stop / fz.battery; exposes [battery, action(up, up_hold,
// down, down_hold, up_release, down_release)]. The originating
// genOnOff/genLevelCtrl commands map to the generic
// kFzCommandOn/Off/Move/Stop set; battery comes from kFzBattery
// (genPowerCfg attr 0x0020). Pure remote — no outbound writes.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

const FzConverter* const kFz_D73743[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D73743[] = { "LIGHTIFY Dimming Switch" };

constexpr Expose kExposes_D73743[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

// Bind genOnOff + genLevelCtrl so the remote's commands reach us, plus
// genPowerCfg for battery reporting. No outbound writes (toZigbee=[]).
constexpr BindingSpec kBindings_D73743[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_D73743{
    .zigbee_models=kModels_D73743, .zigbee_models_count=sizeof(kModels_D73743)/sizeof(kModels_D73743[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="73743", .vendor="Sylvania",
    .meta=nullptr,
    .exposes=kExposes_D73743, .exposes_count=sizeof(kExposes_D73743)/sizeof(kExposes_D73743[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D73743, .from_zigbee_count=sizeof(kFz_D73743)/sizeof(kFz_D73743[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D73743,
    .bindings_count=sizeof(kBindings_D73743)/sizeof(kBindings_D73743[0]),
};

}  // namespace zhc::devices::sylvania
