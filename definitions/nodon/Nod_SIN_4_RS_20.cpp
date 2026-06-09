// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nodon SIN-4-RS-20 — hand-rewrite (calibration manuSpec + tilt).
// Roller shutter relay switch.
//
// z2m: `m.windowCovering({controls:['tilt','lift'], coverMode:true}) +
//       calibrationVerticalRunTimeUp/Down +
//       calibrationRotationRunTimeUp/Down + switchTypeWindowCovering`.
// All four calibrations are u16 manuSpec attrs (0x0001..0x0004) on
// closuresWindowCovering, manu 0x128B; switch_type is enum8 on the
// same cluster (0x1001).
//
// Tilt support: z2m's `controls:['tilt','lift']` exposes both
// `position` and `tilt` and `fz.cover_position_tilt` decodes lift
// (0x0008) + tilt (0x0009). This def wires kFzNodonCoverPositionTilt
// (both halves) plus the generic kTzCoverPositionTilt writer and a
// `tilt` expose. (cover_mode + state still TODO.)
//
// z2m-source: nodon.ts #SIN-4-RS-20 + fromZigbee.ts cover_position_tilt.
#include "definitions/_generic/_shared.hpp"
#include "definitions/nodon/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SIN_4_RS_20[] = {
    &kFzNodonCoverPositionTilt,
};
const TzConverter* const kTz_SIN_4_RS_20[] = {
    &::zhc::generic::kTzCoverPosition,
    &::zhc::generic::kTzCoverPositionTilt,
    &kTzNodonCalibVertUp,
    &kTzNodonCalibVertDown,
    &kTzNodonCalibRotUp,
    &kTzNodonCalibRotDown,
    &kTzNodonSwitchTypeWindowCovering,
};
constexpr const char* kModels_SIN_4_RS_20[] = { "SIN-4-RS-20", "SIN-4-UNK" };

}  // namespace


// Hand-aligned to z2m exposes (cover_mode + state still TODO).
constexpr Expose kAutoExposes[] = {
    {"position",                          ExposeType::Numeric, Access::StateSet, "%",
        nullptr, nullptr, 0},
    {"tilt",                              ExposeType::Numeric, Access::StateSet, "%",
        nullptr, nullptr, 0},
    {"calibration_vertical_run_time_up",  ExposeType::Numeric, Access::StateSet, "10 ms",
        "Manual calibration: vertical run time up.", nullptr, 0, ExposeCategory::Config},
    {"calibration_vertical_run_time_down",ExposeType::Numeric, Access::StateSet, "10 ms",
        "Manual calibration: vertical run time down.", nullptr, 0, ExposeCategory::Config},
    {"calibration_rotation_run_time_up",  ExposeType::Numeric, Access::StateSet, "10 ms",
        "Manual calibration: rotation run time up.", nullptr, 0, ExposeCategory::Config},
    {"calibration_rotation_run_time_down",ExposeType::Numeric, Access::StateSet, "10 ms",
        "Manual calibration: rotation run time down.", nullptr, 0, ExposeCategory::Config},
    {"switch_type",                       ExposeType::Enum,    Access::StateSet, nullptr,
        "Switch type: bistable or monostable.", nullptr, 0, ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SIN_4_RS_20{
    .zigbee_models=kModels_SIN_4_RS_20, .zigbee_models_count=sizeof(kModels_SIN_4_RS_20)/sizeof(kModels_SIN_4_RS_20[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SIN-4-RS-20", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SIN_4_RS_20, .from_zigbee_count=sizeof(kFz_SIN_4_RS_20)/sizeof(kFz_SIN_4_RS_20[0]),
    .to_zigbee=kTz_SIN_4_RS_20, .to_zigbee_count=sizeof(kTz_SIN_4_RS_20)/sizeof(kTz_SIN_4_RS_20[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
