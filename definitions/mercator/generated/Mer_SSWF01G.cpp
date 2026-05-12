// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SSWF01G — hand-rewritten (fan controller).
// AC fan controller.
//
// z2m: fromZigbee:[fz.on_off, fz.fan]
//   toZigbee:  [tz.fan_mode, tz.on_off]
//   exposes:   e.switch()  -> "state"
//              e.fan().withState("fan_state").withModes(
//                ["off","low","medium","high","on"])
//
// 2026-04-28p MERCATOR sweep — kFzFanMode already in place; reshape
// expose set to match z2m (state + fan_mode + fan_state) and tighten
// the fan_mode expose to Numeric (hvacFanCtrl::FanMode is uint8 enum).
//
// Runtime gap: no `kTzFanMode` in `_generic/_shared` so the outbound
// `fan_mode` write is not yet plumbed (PARTIAL — see MERCATOR_PARITY.md).
//
// z2m-source: mercator.ts #SSWF01G.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SSWF01G[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzFanMode,
};
const TzConverter* const kTz_SSWF01G[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_SSWF01G[] = { "TS0501" };
constexpr const char* kManus_SSWF01G[] = {
    "_TZ3210_lzqq3u4r",
    "_TZ3210_4whigl8i",
};

constexpr Expose kExposes_SSWF01G[] = {
    { "state",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "fan_mode",  ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0 },
    { "fan_state", ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SSWF01G[] = {
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0202 },  // hvacFanCtrl
};

}  // namespace

extern const PreparedDefinition kDef_SSWF01G{
    .zigbee_models=kModels_SSWF01G, .zigbee_models_count=sizeof(kModels_SSWF01G)/sizeof(kModels_SSWF01G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SSWF01G, .manufacturer_names_count=sizeof(kManus_SSWF01G)/sizeof(kManus_SSWF01G[0]),
    .model="SSWF01G", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SSWF01G, .exposes_count=sizeof(kExposes_SSWF01G)/sizeof(kExposes_SSWF01G[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SSWF01G, .from_zigbee_count=sizeof(kFz_SSWF01G)/sizeof(kFz_SSWF01G[0]),
    .to_zigbee=kTz_SSWF01G, .to_zigbee_count=sizeof(kTz_SSWF01G)/sizeof(kTz_SSWF01G[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SSWF01G, .bindings_count=sizeof(kBindings_SSWF01G)/sizeof(kBindings_SSWF01G[0]),
};

}  // namespace zhc::devices::mercator
