// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon SIN-4-RS-20_PRO — hand-rewrite (calibration manuSpec).
// Roller shutter relay switch (PRO variant; mirrors SIN-4-RS-20).
//
// z2m-source: nodon.ts #SIN-4-RS-20_PRO.
#include "definitions/_generic/_shared.hpp"
#include "definitions/nodon/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SIN_4_RS_20_PRO[] = {
    &::zhc::generic::kFzCoverPosition,
};
const TzConverter* const kTz_SIN_4_RS_20_PRO[] = {
    &::zhc::generic::kTzCoverPosition,
    &kTzNodonCalibVertUp,
    &kTzNodonCalibVertDown,
    &kTzNodonCalibRotUp,
    &kTzNodonCalibRotDown,
    &kTzNodonSwitchTypeWindowCovering,
};
constexpr const char* kModels_SIN_4_RS_20_PRO[] = { "SIN-4-RS-20_PRO" };

}  // namespace


// Hand-aligned to z2m exposes (cover_mode + tilt + state still TODO).
constexpr Expose kAutoExposes[] = {
    {"position",                          ExposeType::Numeric, Access::StateSet, "%",
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

extern const PreparedDefinition kDef_SIN_4_RS_20_PRO{
    .zigbee_models=kModels_SIN_4_RS_20_PRO, .zigbee_models_count=sizeof(kModels_SIN_4_RS_20_PRO)/sizeof(kModels_SIN_4_RS_20_PRO[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SIN-4-RS-20_PRO", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SIN_4_RS_20_PRO, .from_zigbee_count=sizeof(kFz_SIN_4_RS_20_PRO)/sizeof(kFz_SIN_4_RS_20_PRO[0]),
    .to_zigbee=kTz_SIN_4_RS_20_PRO, .to_zigbee_count=sizeof(kTz_SIN_4_RS_20_PRO)/sizeof(kTz_SIN_4_RS_20_PRO[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
