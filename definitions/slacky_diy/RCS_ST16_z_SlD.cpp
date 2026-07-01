// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SlackyDiy RCS-ST16-z-SlD — EKF socket with power monitoring
// (custom firmware), zigbeeModel "RCS-ST16-z-SlD".
//
// z2m-source: slacky_diy.ts #RCS-ST16-z-SlD.  z2m bundles
//   m.onOff({powerOnBehavior:true}) + m.electricityMeter({current:{divisor:100},
//   voltage:{divisor:100}, power:{divisor:1}, energy:{divisor:1000000}})
// on the standard genOnOff (0x0006), seMetering (0x0702) and
// haElectricalMeasurement (0x0B04) clusters, then adds a large set of
// manufacturer-specific controls on custom attributes of the custom-augmented
// seMetering / haElectricalMeasurement clusters: key_lock, led_control,
// protect_control, automatic_restart, voltage_min/max, current_max, power_max,
// time_reload, plus an energyReset command.  Those ride manuf-specific
// attributes / a custom seMetering command for which the ZHC runtime has no
// generic decoder, so they are deferred here — identically to the shipped
// sibling TS011F_plug-SlD (generated/Sla_TS011F_plug_SlD.cpp).
//
// The generic kFzOnOff / kFzMetering / kFzElectricalMeasurement + kTzOnOff
// cover the wire-monitoring surface (state + energy/power/voltage/current),
// which is the parity floor this socket class ships.  NOTE for the build /
// hardware gate: z2m pins non-standard divisors (energy /1e6, power /1); the
// generic electrical/metering readers honour the device-reported divisor
// attributes, matching the sibling plug — verify scaling on hardware.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {

const FzConverter* const kFz_RCS_ST16_z_SlD[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_RCS_ST16_z_SlD[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_RCS_ST16_z_SlD[] = { "RCS-ST16-z-SlD" };

constexpr Expose kExposes_RCS_ST16_z_SlD[] = {
    {"state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",  ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"power",   ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_RCS_ST16_z_SlD[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0702},   // seMetering
    {1, 0x0B04},   // haElectricalMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_RCS_ST16_z_SlD{
    .zigbee_models=kModels_RCS_ST16_z_SlD, .zigbee_models_count=sizeof(kModels_RCS_ST16_z_SlD)/sizeof(kModels_RCS_ST16_z_SlD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RCS-ST16-z-SlD", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kExposes_RCS_ST16_z_SlD, .exposes_count=sizeof(kExposes_RCS_ST16_z_SlD)/sizeof(kExposes_RCS_ST16_z_SlD[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RCS_ST16_z_SlD, .from_zigbee_count=sizeof(kFz_RCS_ST16_z_SlD)/sizeof(kFz_RCS_ST16_z_SlD[0]),
    .to_zigbee=kTz_RCS_ST16_z_SlD, .to_zigbee_count=sizeof(kTz_RCS_ST16_z_SlD)/sizeof(kTz_RCS_ST16_z_SlD[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_RCS_ST16_z_SlD, .bindings_count=sizeof(kBindings_RCS_ST16_z_SlD)/sizeof(kBindings_RCS_ST16_z_SlD[0]),
};

}  // namespace zhc::devices::slacky_diy
