// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Profalux MOT-C1ZxxC/F — cover with lift + tilt (parity fix).
//
// z2m wires this cover as:
//   fz: command_cover_close, command_cover_open, cover_position_tilt
//   tz: cover_state, cover_position_tilt
//   exposes: cover_position_tilt() when the motor is configured as a
//            tilt-capable BSO (manuSpecificProfalux1 motorCoverType==1),
//            else cover_position().
//   endpoint: () => ({default: 2})  — the cover lives on EP2, bound on
//            closuresWindowCovering (0x0102).
//
// The auto-port mis-modelled this as an on/off + position device: it wired
// the generic kFzOnOff / kTzOnOff (genOnOff) behind a phantom `state`
// expose, dropped the tilt half entirely, bound genOnOff (0x0006) on EP1,
// and never set the EP2 default. genOnOff is wrong (z2m's tz.cover_state
// drives closuresWindowCovering open/close, NOT genOnOff) and the tilt
// channel was lost.
//
// Fixed:
//   - dropped the phantom on/off (kFzOnOff/kTzOnOff + `state` expose),
//   - decode lift via kFzCoverPosition (attr 0x0008 -> `position`) and tilt
//     via kFzCoverTilt (attr 0x0009 -> `tilt`),
//   - encode via kTzCoverState (state OPEN/CLOSE/STOP -> 0x0102 cmd
//     0x00/0x01/0x02), kTzCoverPosition (`position` -> goToLift) and
//     kTzCoverPositionTilt (`tilt` -> goToTilt),
//   - exposed both `position` and `tilt` (static superset — firmware can't
//     read motorCoverType at definition time; the tilt channel is harmless
//     on a non-BSO motor that never reports attr 0x0009),
//   - bound closuresWindowCovering (0x0102) on EP2 and set
//     default_endpoint=2 so outbound commands route to the cover EP.
// z2m-source: profalux.ts #MOT-C1ZxxC/F.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::profalux {
namespace {
const FzConverter* const kFz_MOT_C1ZxxC_F[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzCoverTilt,
};
const TzConverter* const kTz_MOT_C1ZxxC_F[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPosition,
    &::zhc::generic::kTzCoverPositionTilt,
};
constexpr const char* kModels_MOT_C1ZxxC_F[] = { "MOT-C1Z06C", "MOT-C1Z10C", "MOT-C1Z20C", "MOT-C1Z30C", "MOT-C1Z06F", "MOT-C1Z10F", "MOT-C1Z20F", "MOT-C1Z30F" };

constexpr Expose kExp_MOT_C1ZxxC_F[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"tilt",     ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
};

// z2m binds closuresWindowCovering on EP2 (the cover lives on EP2).
constexpr BindingSpec kBind_MOT_C1ZxxC_F[] = {
    {2, 0x0102},
};
}  // namespace

extern const PreparedDefinition kDef_MOT_C1ZxxC_F{
    .zigbee_models=kModels_MOT_C1ZxxC_F, .zigbee_models_count=sizeof(kModels_MOT_C1ZxxC_F)/sizeof(kModels_MOT_C1ZxxC_F[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MOT-C1ZxxC/F", .vendor="Profalux",
    .meta=nullptr, .exposes=kExp_MOT_C1ZxxC_F, .exposes_count=sizeof(kExp_MOT_C1ZxxC_F)/sizeof(kExp_MOT_C1ZxxC_F[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MOT_C1ZxxC_F, .from_zigbee_count=sizeof(kFz_MOT_C1ZxxC_F)/sizeof(kFz_MOT_C1ZxxC_F[0]),
    .to_zigbee=kTz_MOT_C1ZxxC_F, .to_zigbee_count=sizeof(kTz_MOT_C1ZxxC_F)/sizeof(kTz_MOT_C1ZxxC_F[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_MOT_C1ZxxC_F, .bindings_count=sizeof(kBind_MOT_C1ZxxC_F)/sizeof(kBind_MOT_C1ZxxC_F[0]),
    .default_endpoint=2,
};

}  // namespace zhc::devices::profalux
