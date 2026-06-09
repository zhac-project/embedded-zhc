// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Siglis ZFP-1A-CH — hand-rewritten from auto-generated stub.
// zigfred plus smart in-wall switch.
// z2m-source: siglis.ts #ZFP-1A-CH.
//
// Endpoint map (z2m endpoint() block):
//   l1 = 5   (zigfredEndpoint, control + front-surface RGB LED + button events)
//   l2 = 7   (dimmer 1; dimmable when dimmer_1_dimming_enabled)
//   l3 = 8   (dimmer 2)
//   l4 = 9   (dimmer 3)
//   l5 = 10  (dimmer 4)
//   l6 = 11  (cover 1; tilt when cover_1_tilt_enabled)
//   l7 = 12  (cover 2; tilt when cover_2_tilt_enabled)
//
// Button events ride on manuSpecificSiglisZigfred (cluster 0xFC42).
// Cover position/tilt is closuresWindowCovering (cluster 0x0102).
//
// Original auto-generated bundle was wrong: only state+brightness+position
// on EP1, no per-endpoint exposes, no color, no action, no manu-cluster
// binding, only single cover instead of two.
//
// Tier-2 parity fixes (feat/siglis-parity):
//   * EP5 (`l1`) was absent from endpoint_map, so the front-surface RGB
//     LED's state/brightness/color reports never picked up the `_l1`
//     suffix and could never match the declared state_l1/brightness_l1/
//     color_l1 exposes. z2m maps `l1: zigfredEndpoint(=5)`. Added below.
//   * Button events on the manu cluster now decode via
//     siglis::kFzZigfredButtonEvent (was a documented TODO no-op).
#include "definitions/_generic/_shared.hpp"
#include "definitions/siglis/_shared.hpp"

namespace zhc::devices::siglis {
namespace {

// z2m-source: fromZigbee=[zifgredFromZigbeeButtonEvent, fz.color_colortemp,
// fz.on_off, fz.brightness, fz.level_config, fz.power_on_behavior,
// fz.cover_position_tilt].
//
// kFzZigfredButtonEvent decodes the manu-cluster button presses (0xFC42
// cmd 0x02) into `action` (button_<n>_<event>).
//
// TODO(parity): kFzLevelConfig, kFzPowerOnBehavior1 have no generic
// equivalents — level-config round-trips currently no-op. Cover tilt
// rides the same closuresWindowCovering cluster as position; the generic
// kFzCoverPosition reads only `currentPositionLift`, so tilt reports are
// also no-op until a tilt-aware variant lands.
const FzConverter* const kFz_ZFP_1A_CH[] = {
    &::zhc::siglis::kFzZigfredButtonEvent,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzCoverPosition,
};

// z2m-source: toZigbee=[tz.light_color, tz.ignore_transition, tz.ignore_rate,
// tz.light_brightness_move, tz.light_brightness_step, tz.level_config,
// tz.power_on_behavior, tz.light_hue_saturation_move,
// tz.light_hue_saturation_step, tz.light_color_options, tz.light_color_mode,
// coverAndLightToZigbee] (the last is a wrapper that fans tz.cover_state +
// tz.light_onoff_brightness based on endpoint clusters).
//
// TODO(parity): the move/step/hue-saturation/level-config/color-options
// helpers do not have generic equivalents yet. coverAndLightToZigbee's
// per-endpoint dispatching collapses to the generic kTzOnOff/kTzBrightness
// for lights and kTzCoverPosition for covers below.
const TzConverter* const kTz_ZFP_1A_CH[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzCoverPosition,
};

constexpr const char* kModels_ZFP_1A_CH[] = { "zigfred plus" };

// `l1`=EP5 MUST be present so front-surface RGB reports get the `_l1`
// suffix (matching the state_l1/brightness_l1/color_l1 exposes). z2m
// includes it via `l1: zigfredEndpoint`.
constexpr ::zhc::EndpointLabel kEndpoints_ZFP_1A_CH[] = { {"l1", 5}, {"l2", 7}, {"l3", 8}, {"l4", 9}, {"l5", 10}, {"l6", 11}, {"l7", 12} };

}  // namespace

// Hand-curated exposes — multiEndpoint expanded.
// Same caveat as ZFU-1D-CH: ZHC has no per-device option toggles, so we
// publish the union of all possible exposes (4 dimmers + 2 covers + LED).
constexpr Expose kAutoExposes[] = {
    // Buttons (12 actions on l1 / EP5).
    {"action", ExposeType::Enum, Access::State, nullptr, "button_<n>_<event>", nullptr, 0},
    // l1 = EP5 — front-surface RGB LED.
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_l1", ExposeType::String, Access::StateSet, nullptr, "x,y", nullptr, 0},
    // l2 = EP7 — dimmer 1.
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // l3 = EP8 — dimmer 2.
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l3", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // l4 = EP9 — dimmer 3.
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l4", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // l5 = EP10 — dimmer 4.
    {"state_l5", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l5", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // l6 = EP11 — cover 1 (position; tilt is TODO).
    {"state_l6", ExposeType::Enum, Access::StateSet, nullptr, "OPEN/CLOSE/STOP", nullptr, 0},
    {"position_l6", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    // l7 = EP12 — cover 2.
    {"state_l7", ExposeType::Enum, Access::StateSet, nullptr, "OPEN/CLOSE/STOP", nullptr, 0},
    {"position_l7", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
};

// Bindings: replicate z2m's per-EP `reporting.bind(...)` set. The z2m
// configure block only binds endpoints whose option flag is "true", but we
// pre-bind everything so reports flow on first contact regardless of the
// option-toggle UX (which doesn't yet exist on ZHAC).
constexpr BindingSpec kAutoBindings[] = {
    {5,  0x0006}, // genOnOff       — front-surface
    {5,  0x0008}, // genLevelCtrl
    {5,  0x0300}, // genColorCtrl
    {5,  0xFC42}, // manuSpecificSiglisZigfred
    {7,  0x0006}, // dimmer 1 on/off
    {7,  0x0008}, // dimmer 1 level
    {8,  0x0006}, // dimmer 2
    {8,  0x0008},
    {9,  0x0006}, // dimmer 3
    {9,  0x0008},
    {10, 0x0006}, // dimmer 4
    {10, 0x0008},
    {11, 0x0102}, // cover 1
    {12, 0x0102}, // cover 2
};

extern const PreparedDefinition kDef_ZFP_1A_CH{
    .zigbee_models=kModels_ZFP_1A_CH, .zigbee_models_count=sizeof(kModels_ZFP_1A_CH)/sizeof(kModels_ZFP_1A_CH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZFP-1A-CH", .vendor="Siglis",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZFP_1A_CH, .from_zigbee_count=sizeof(kFz_ZFP_1A_CH)/sizeof(kFz_ZFP_1A_CH[0]),
    .to_zigbee=kTz_ZFP_1A_CH, .to_zigbee_count=sizeof(kTz_ZFP_1A_CH)/sizeof(kTz_ZFP_1A_CH[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ZFP_1A_CH,
    .endpoint_map_count = sizeof(kEndpoints_ZFP_1A_CH)/sizeof(kEndpoints_ZFP_1A_CH[0]),
};

}  // namespace zhc::devices::siglis
