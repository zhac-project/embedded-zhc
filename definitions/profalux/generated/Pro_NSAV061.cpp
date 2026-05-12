// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Profalux NSAV061 — older covers using genLevelCtrl as cover.
// Cover (older). z2m matches on fingerprint only (no zigbeeModel) so this
// def carries an empty zigbee_models list — runtime falls through and
// matches on the platform's fingerprint table.  z2m wires:
//   fz: cover_position_via_brightness, cover_state_via_onoff
//   tz: cover_via_brightness
// We approximate with kFzBrightness (genLevelCtrl currentLevel → "brightness")
// + kFzOnOff (genOnOff → "state") + kTzBrightness on the read/write side.
// A future tier-3 upgrade can rewrite the FZ to emit `position` directly.
// z2m-source: profalux.ts #NSAV061.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::profalux {
namespace {
const FzConverter* const kFz_NSAV061[] = {
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_NSAV061[] = {
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzOnOff,
};

constexpr Expose kExp_NSAV061[] = {
    {"position", ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0},
    {"state",    ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_NSAV061[] = {
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0006},   // genOnOff
};
}  // namespace

extern const PreparedDefinition kDef_NSAV061{
    .zigbee_models=nullptr, .zigbee_models_count=0,
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
