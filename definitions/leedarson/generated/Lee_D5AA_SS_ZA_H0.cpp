// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Leedarson 5AA-SS-ZA-H0 — hand-rewritten 2026-04-29.
// Motion sensor (PIR)
// z2m-source: leedarson.ts #5AA-SS-ZA-H0.
// Mapping: fingerprint=[{modelID:"ZHA-PIRSensor", manufacturerName:"Leedarson"}]
//          fromZigbee=[fz.occupancy, fz.ignore_occupancy_report]
//          + extend=[m.illuminance()].
//          fz.ignore_occupancy_report is a generic noise filter (no exposes,
//          no expressed converter required) — only kFzOccupancy + kFzIlluminance
//          are wired here.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leedarson {
namespace {
const FzConverter* const kFz_D5AA_SS_ZA_H0[] = {
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_D5AA_SS_ZA_H0[] = { "ZHA-PIRSensor" };
constexpr const char* kManufacturers_D5AA_SS_ZA_H0[] = { "Leedarson" };

constexpr Expose kExposes_D5AA_SS_ZA_H0[] = {
    { "occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0 },
    { "illuminance", ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D5AA_SS_ZA_H0[] = {
    { 1, 0x0406 },  // msOccupancySensing
    { 1, 0x0400 },  // msIlluminanceMeasurement
};
}  // namespace

extern const PreparedDefinition kDef_D5AA_SS_ZA_H0{
    .zigbee_models=kModels_D5AA_SS_ZA_H0,
    .zigbee_models_count=sizeof(kModels_D5AA_SS_ZA_H0)/sizeof(kModels_D5AA_SS_ZA_H0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManufacturers_D5AA_SS_ZA_H0,
    .manufacturer_names_count=sizeof(kManufacturers_D5AA_SS_ZA_H0)/sizeof(kManufacturers_D5AA_SS_ZA_H0[0]),
    .model="5AA-SS-ZA-H0", .vendor="Leedarson",
    .meta=nullptr, .exposes=kExposes_D5AA_SS_ZA_H0, .exposes_count=sizeof(kExposes_D5AA_SS_ZA_H0)/sizeof(kExposes_D5AA_SS_ZA_H0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D5AA_SS_ZA_H0, .from_zigbee_count=sizeof(kFz_D5AA_SS_ZA_H0)/sizeof(kFz_D5AA_SS_ZA_H0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D5AA_SS_ZA_H0, .bindings_count=sizeof(kBindings_D5AA_SS_ZA_H0)/sizeof(kBindings_D5AA_SS_ZA_H0[0]),
};

}  // namespace zhc::devices::leedarson
