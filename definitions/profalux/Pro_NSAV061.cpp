// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Profalux NSAV061 — older cover driven via genLevelCtrl (parity fix).
//
// These older covers do NOT expose closuresWindowCovering; z2m matches them
// on fingerprint only (no zigbeeModel), so this def carries an empty
// zigbee_models list and the runtime falls through to the platform's
// fingerprint table. z2m wires:
//   fz: cover_position_via_brightness  (genLevelCtrl currentLevel -> position+state)
//       cover_state_via_onoff          (genOnOff onOff -> state OPEN/CLOSE)
//   tz: cover_via_brightness           (position -> genLevelCtrl moveToLevel)
//   exposes: cover_position().setAccess("state", ea.ALL)  (state + position)
//
// The auto-port approximated this with the generic kFzBrightness (which
// emits the raw `brightness` key, NOT scaled `position`) + kFzOnOff (boolean
// `state`, not OPEN/CLOSE) + kTzBrightness (claims `brightness`, not
// `position`). Result: the `position` expose decoded to NOTHING and `state`
// was a bare on/off boolean — the cover's real position never surfaced.
//
// Graduated to wire the vendor cover-via-brightness converters
// (profalux/_shared.cpp, modelled on the iluminize 5128.10 precedent):
//   - kFzNsav061PositionViaBrightness: currentLevel(0..255) -> position(0..100)
//     + derived OPEN/CLOSE state,
//   - kFzNsav061StateViaOnOff:         onOff -> state OPEN/CLOSE,
//   - kTzNsav061PositionViaBrightness: position(0..100) -> genLevelCtrl
//     moveToLevelWithOnOff(level 0..255).
// `state` is reported (read) only here: z2m's state-write path drives the
// cover via genLevelCtrl level / stop, which the position writer already
// covers — exposing a separate settable `state` would only no-op.
// z2m-source: profalux.ts #NSAV061 + converters cover_position_via_brightness
//             / cover_state_via_onoff / cover_via_brightness.
#include "definitions/_generic/_shared.hpp"
#include "definitions/profalux/_shared.hpp"

namespace zhc::devices::profalux {
namespace {
const FzConverter* const kFz_NSAV061[] = {
    &kFzNsav061PositionViaBrightness,   // genLevelCtrl -> position + state
    &kFzNsav061StateViaOnOff,           // genOnOff     -> state
};
const TzConverter* const kTz_NSAV061[] = {
    &kTzNsav061PositionViaBrightness,   // position -> genLevelCtrl level
};

// e.cover_position().setAccess("state", ea.ALL) ⇒ state (OPEN/CLOSE) +
// position (%). `state` is read-only here (see header note).
constexpr Expose kExp_NSAV061[] = {
    {"state",    ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, ::zhc::Access::StateSet, "%",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_NSAV061[] = {
    {1, 0x0008},   // genLevelCtrl  (cover_position_via_brightness)
    {1, 0x0006},   // genOnOff      (cover_state_via_onoff)
};
}  // namespace

extern const PreparedDefinition kDef_NSAV061{
    .zigbee_models=nullptr,.zigbee_models_count=0,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NSAV061", .vendor="Profalux",
    .meta=nullptr, .exposes=kExp_NSAV061, .exposes_count=sizeof(kExp_NSAV061)/sizeof(kExp_NSAV061[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NSAV061, .from_zigbee_count=sizeof(kFz_NSAV061)/sizeof(kFz_NSAV061[0]),
    .to_zigbee=kTz_NSAV061, .to_zigbee_count=sizeof(kTz_NSAV061)/sizeof(kTz_NSAV061[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_NSAV061,
    .bindings_count=sizeof(kBind_NSAV061)/sizeof(kBind_NSAV061[0]),
};

}  // namespace zhc::devices::profalux
