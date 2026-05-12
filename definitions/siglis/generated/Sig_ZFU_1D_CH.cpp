// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Siglis ZFU-1D-CH — hand-rewritten from auto-generated stub.
// zigfred uno smart in-wall switch.
// z2m-source: siglis.ts #ZFU-1D-CH.
//
// Endpoint map (z2m endpoint() block):
//   l1 = 5  (zigfredEndpoint, control + front-surface RGB LED + button events)
//   l2 = 6  (relay)
//   l3 = 7  (dimmer; dimmable when dimmer_dimming_enabled)
//
// Front-surface l1 is RGB color-xy + brightness on genOnOff/genLevelCtrl/genColorCtrl.
// Button events ride on manuSpecificSiglisZigfred (cluster id 0xFC42, command
// `commandSiglisZigfredButtonEvent`) and emit "button_<n>_<event>" actions.
//
// Original auto-generated bundle was wrong: only state+brightness on EP1, no
// per-endpoint exposes, no color, no action, no manu-cluster binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::siglis {
namespace {

// z2m-source: fromZigbee=[zifgredFromZigbeeButtonEvent, fz.color_colortemp,
// fz.on_off, fz.brightness, fz.level_config, fz.power_on_behavior].
//
// TODO(parity): kFzZigfredButtonEvent (manu cluster 0xFC42), kFzLevelConfig,
// kFzPowerOnBehavior1 are not yet in the generic shared library. We bind
// the on/off, brightness and color generics so the device's lighting state
// is still observable; button events and level configuration round-trips
// currently no-op.
const FzConverter* const kFz_ZFU_1D_CH[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};

// z2m-source: toZigbee=[tz.light_onoff_brightness, tz.light_color,
// tz.ignore_transition, tz.ignore_rate, tz.light_brightness_move,
// tz.light_brightness_step, tz.level_config, tz.power_on_behavior,
// tz.light_hue_saturation_move, tz.light_hue_saturation_step,
// tz.light_color_options, tz.light_color_mode].
//
// TODO(parity): only the on/off + brightness + color core is wired. The
// move/step/hue-saturation/level-config/color-options helpers do not have
// generic equivalents yet — multi-step animations and option writes will
// be no-ops until a Tier-3 pass adds them.
const TzConverter* const kTz_ZFU_1D_CH[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};

constexpr const char* kModels_ZFU_1D_CH[] = { "zigfred uno" };

constexpr ::zhc::EndpointLabel kEndpoints_ZFU_1D_CH[] = { {"l2", 6}, {"l3", 7} };

}  // namespace

// Hand-curated exposes — multiEndpoint expanded.
// z2m gates each entry on a `*_enabled` option (front_surface_enabled,
// relay_enabled, dimmer_enabled, dimmer_dimming_enabled). ZHC has no
// runtime expose toggles, so we publish the union of all possible
// exposes; missing endpoints simply won't report.
//
// Naming follows z2m's withEndpoint() convention: "<key>_<endpoint-name>".
constexpr Expose kAutoExposes[] = {
    // Buttons (12 actions on l1 / EP5, manu-cluster) — currently undecoded
    // (see kFz TODO above); declared so consumers can subscribe early.
    {"action", ExposeType::Enum, Access::State, nullptr, "button_<n>_<event>", nullptr, 0},
    // l1 = EP5 — front-surface RGB LED + brightness.
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_l1", ExposeType::String, Access::StateSet, nullptr, "x,y", nullptr, 0},
    // l2 = EP6 — relay (switch only).
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // l3 = EP7 — dimmer (switch + brightness; brightness only when
    // dimmer_dimming_enabled at the device).
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

// Bindings: replicate z2m's per-EP `reporting.bind(...)` set.
//   EP5: genOnOff, genLevelCtrl, genColorCtrl, manuSpecificSiglisZigfred (0xFC42).
//   EP6: genOnOff.
//   EP7: genOnOff, genLevelCtrl.
//
// TODO(parity): cluster 0xFC42 is the siglis manu cluster; binding it lets
// the radio surface button-event commandsResponse frames even before a
// dedicated FzConverter exists.
constexpr BindingSpec kAutoBindings[] = {
    {5, 0x0006},  // genOnOff       — control / front-surface
    {5, 0x0008},  // genLevelCtrl
    {5, 0x0300},  // genColorCtrl
    {5, 0xFC42},  // manuSpecificSiglisZigfred
    {6, 0x0006},  // relay
    {7, 0x0006},  // dimmer
    {7, 0x0008},  // dimmer level
};

extern const PreparedDefinition kDef_ZFU_1D_CH{
    .zigbee_models=kModels_ZFU_1D_CH, .zigbee_models_count=sizeof(kModels_ZFU_1D_CH)/sizeof(kModels_ZFU_1D_CH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZFU-1D-CH", .vendor="Siglis",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZFU_1D_CH, .from_zigbee_count=sizeof(kFz_ZFU_1D_CH)/sizeof(kFz_ZFU_1D_CH[0]),
    .to_zigbee=kTz_ZFU_1D_CH, .to_zigbee_count=sizeof(kTz_ZFU_1D_CH)/sizeof(kTz_ZFU_1D_CH[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ZFU_1D_CH,
    .endpoint_map_count = sizeof(kEndpoints_ZFU_1D_CH)/sizeof(kEndpoints_ZFU_1D_CH[0]),
};

}  // namespace zhc::devices::siglis
