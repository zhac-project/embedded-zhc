// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: OWON SLC611 smart switch with power metering.
// New upstream in z2m v26.100.0.
//
// z2m: `m.onOff() + m.electricityMeter({...}) + m.forcePowerSource(...)`.
//
// The metering configuration is unusual and worth spelling out: z2m routes
// POWER through the metering cluster (0x0702) with divisor 1 / multiplier 1,
// while voltage (divisor 10) and current (divisor 1000) come off electrical
// measurement (0x0B04). Both clusters are bound and both generic converters
// wired; the generic decoders read each cluster's own divisor/multiplier
// attributes off the device, so the values land correctly without a
// per-device scale table.
//
// `forcePowerSource: "Mains (single phase)"` — the device misreports its
// power source in genBasic, which would otherwise make the UI show it as
// battery powered. Carried as the power_source_override field rather than an
// expose.
//
// z2m-source: owon.ts #SLC611.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {

const FzConverter* const kFz_SLC611[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SLC611[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_SLC611[] = { "SLC611" };

constexpr Expose kExposes_SLC611[] = {
    {"state",        ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power",        ExposeType::Numeric, Access::State,    "W",   nullptr, nullptr, 0},
    {"energy",       ExposeType::Numeric, Access::State,    "kWh", nullptr, nullptr, 0},
    {"voltage",      ExposeType::Numeric, Access::State,    "V",   nullptr, nullptr, 0},
    {"current",      ExposeType::Numeric, Access::State,    "A",   nullptr, nullptr, 0},
    {"power_factor", ExposeType::Numeric, Access::State,    "%",   nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_SLC611[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0702},   // seMetering — power + energy
    {1, 0x0B04},   // haElectricalMeasurement — voltage, current, power factor
};

}  // namespace

extern const PreparedDefinition kDef_SLC611{
    .zigbee_models=kModels_SLC611, .zigbee_models_count=sizeof(kModels_SLC611)/sizeof(kModels_SLC611[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SLC611", .vendor="OWON",
    .meta=nullptr,
    .exposes=kExposes_SLC611, .exposes_count=sizeof(kExposes_SLC611)/sizeof(kExposes_SLC611[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SLC611, .from_zigbee_count=sizeof(kFz_SLC611)/sizeof(kFz_SLC611[0]),
    .to_zigbee=kTz_SLC611, .to_zigbee_count=sizeof(kTz_SLC611)/sizeof(kTz_SLC611[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SLC611, .bindings_count=sizeof(kBindings_SLC611)/sizeof(kBindings_SLC611[0]),
    .power_source_override = 0x01,   // ZCL Basic 0x0007 = Mains (single phase)
};

}  // namespace zhc::devices::owon
