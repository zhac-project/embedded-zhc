// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Itcommander ITCMDR_Click — wrong-bundle (action) fix.
// Button
// z2m-source: ITCommander.ts #ITCMDR_Click (fz.itcmdr_clicks).
//
// Gap (class e — phantom on/off bundle / dead state): the auto-port
// wired this BUTTON as a controllable genOnOff switch (`kFzOnOff` +
// `kTzOnOff` + a `state` StateSet expose). z2m's role is `action` via
// `fz.itcmdr_clicks`, which decodes genMultistateInput PresentValue
// into single/double/triple/quadruple/hold/release/many. Re-port to
// the vendor `kFzItcmdrClicks` converter, expose `action` (+battery,
// voltage), and drop the spurious to_zigbee/state.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ITCommander/_shared.hpp"

namespace zhc::devices::ITCommander {
namespace {
const FzConverter* const kFz_ITCMDR_Click[] = {
    &::zhc::ITCommander::kFzItcmdrClicks,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ITCMDR_Click[] = { "ITCMDR_Click" };

}  // namespace


// z2m exposes: e.action(["single","double","triple","hold","release",
// "many"]), e.battery(), e.voltage(). (z2m's fz lookup also emits
// "quadruple" for presentValue 4; harmless superset of the action
// enum.)
const char* const kActionValues[] = {
    "single", "double", "triple", "quadruple", "hold", "release", "many",
};
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0012},   // genMultistateInput (button presses)
    {1, 0x0001},   // genPowerCfg (battery)
};

extern const PreparedDefinition kDef_ITCMDR_Click{
    .zigbee_models=kModels_ITCMDR_Click, .zigbee_models_count=sizeof(kModels_ITCMDR_Click)/sizeof(kModels_ITCMDR_Click[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ITCMDR_Click", .vendor="Itcommander",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ITCMDR_Click, .from_zigbee_count=sizeof(kFz_ITCMDR_Click)/sizeof(kFz_ITCMDR_Click[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ITCommander
