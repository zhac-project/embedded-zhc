// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon SLC603 — auto-generated.
// Zigbee remote dimmer
// z2m-source: owon.ts #SLC603.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
// Battery-powered scene/dimmer remote — emits server-side commands (toggle, step,
// step color temperature) toward the coordinator. NO on/off attribute, NO tz.
const FzConverter* const kFz_SLC603[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStepColorTemp,
};
constexpr const char* kModels_SLC603[] = { "SLC603" };

}  // namespace


// z2m: e.battery(), e.battery_low(), e.action([toggle, brightness_step_up/down, color_temperature_step_up/down]).
constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action",      ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff (toggle command source)
    {1, 0x0008},  // genLevelCtrl (step up/down)
    {1, 0x0300},  // lightingColorCtrl (step color temperature)
};

extern const PreparedDefinition kDef_SLC603{
    .zigbee_models=kModels_SLC603, .zigbee_models_count=sizeof(kModels_SLC603)/sizeof(kModels_SLC603[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SLC603", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SLC603, .from_zigbee_count=sizeof(kFz_SLC603)/sizeof(kFz_SLC603[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
