// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Pushok POK017 — battery powered greenhouse vent.
// z2m-source: pushok.ts #POK017.
// Graduated from generated/: z2m wires m.windowCovering({controls:["lift"]})
// — a cover with position on closuresWindowCovering(0x0102), decoded via
// fz.cover_position_tilt + tz.cover_state/cover_position_tilt. A prior port
// over-corrected an earlier mis-emit by removing the cover ENTIRELY, leaving
// the vent battery-only with NO actuator channel. Restored the cover:
// kFzCoverPosition + kTzCoverState/kTzCoverPositionLift, with `state`+
// `position` exposes and the 0x0102 binding. heat_sensitivity/status ride
// genMultistateOutput/Input (no generic decoder) → INFRA, exposed only.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK017[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_POK017[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPositionLift,
};

constexpr const char* kModels_POK017[] = { "POK017" };

}  // namespace


// z2m: m.windowCovering -> e.cover().withPosition() (state + position) +
// battery + heat_sensitivity enum + status enum. The two vendor enums sit on
// genMultistateOutput and genMultistateInput respectively; exposed for UI
// (no generic decoder).
constexpr Expose kAutoExposes[] = {
    {"state",    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"heat_sensitivity", ExposeType::Enum, Access::StateSet, nullptr,
        "Heat sensitivity level for automatic vent opening", nullptr, 0,
        ExposeCategory::Config},
    {"status", ExposeType::Enum, Access::State, nullptr,
        "Actual window status", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},  // closuresWindowCovering
    {1, 0x0001},  // genPowerCfg
};

extern const PreparedDefinition kDef_POK017{
    .zigbee_models=kModels_POK017, .zigbee_models_count=sizeof(kModels_POK017)/sizeof(kModels_POK017[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK017", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK017, .from_zigbee_count=sizeof(kFz_POK017)/sizeof(kFz_POK017[0]),
    .to_zigbee=kTz_POK017, .to_zigbee_count=sizeof(kTz_POK017)/sizeof(kTz_POK017[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
