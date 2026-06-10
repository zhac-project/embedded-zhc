// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Iluminize 5128.10 — roller-shutter relay with level control.
//
// z2m wires this device as a brightness-driven cover:
//   fromZigbee = [cover_position_via_brightness,   // genLevelCtrl currentLevel → position+state
//                 cover_state_via_onoff,           // genOnOff onOff → state
//                 cover_position_tilt]             // closuresWindowCovering lift → position
//   toZigbee   = [cover_state,                     // closuresWindowCovering up/down/stop
//                 cover_via_brightness]            // genLevelCtrl moveToLevelWithOnOff
//   exposes    = [cover_position()]                // state + position
//
// The auto-generated port bound only the generic kFzCoverPosition
// (closuresWindowCovering attr 0x0008) and exposed `position` alone — so a
// real device, which reports its open percentage on genLevelCtrl.currentLevel
// and OPEN/CLOSE on genOnOff, decoded to NOTHING and never surfaced state.
// Graduated to wire the brightness/onoff legs (vendor-shared converters) plus
// the windowCovering lift fallback, and to expose `state` like e.cover_position().
// z2m-source: iluminize.ts #5128.10 + converters/fromZigbee.ts
//             cover_position_via_brightness / cover_state_via_onoff /
//             cover_position_tilt + toZigbee.ts cover_state / cover_via_brightness.
#include "definitions/_generic/_shared.hpp"
#include "definitions/iluminize/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {
const FzConverter* const kFz_D5128_10[] = {
    &::zhc::devices::iluminize::kFzIluCoverViaBrightness,   // genLevelCtrl → position + state
    &::zhc::devices::iluminize::kFzIluCoverStateViaOnOff,   // genOnOff     → state
    &::zhc::generic::kFzCoverPosition,                      // windowCovering lift fallback
};
const TzConverter* const kTz_D5128_10[] = {
    &::zhc::generic::kTzCoverState,                         // state → windowCovering up/down/stop
    &::zhc::devices::iluminize::kTzIluCoverViaBrightness,   // position → genLevelCtrl level
};
constexpr const char* kModels_D5128_10[] = { "5128.10" };

// e.cover_position() ⇒ state (OPEN/CLOSE) + position (%).
constexpr Expose kExposes_D5128_10[] = {
    {"state",    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D5128_10[] = {
    {1, 0x0006},   // genOnOff               (cover_state_via_onoff)
    {1, 0x0008},   // genLevelCtrl           (cover_position_via_brightness)
    {1, 0x0102},   // closuresWindowCovering (cover_position_tilt lift)
};

}  // namespace

extern const PreparedDefinition kDef_D5128_10{
    .zigbee_models=kModels_D5128_10, .zigbee_models_count=sizeof(kModels_D5128_10)/sizeof(kModels_D5128_10[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="5128.10", .vendor="Iluminize",
    .meta=nullptr, .exposes=kExposes_D5128_10, .exposes_count=sizeof(kExposes_D5128_10)/sizeof(kExposes_D5128_10[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D5128_10, .from_zigbee_count=sizeof(kFz_D5128_10)/sizeof(kFz_D5128_10[0]),
    .to_zigbee=kTz_D5128_10, .to_zigbee_count=sizeof(kTz_D5128_10)/sizeof(kTz_D5128_10[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D5128_10, .bindings_count=sizeof(kBindings_D5128_10)/sizeof(kBindings_D5128_10[0]),
};

}  // namespace zhc::devices::iluminize
