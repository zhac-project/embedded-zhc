// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lidl HG08673 — hand-maintained parity override.
// Silvercrest smart plug with power monitoring (EU, FR).
//
// Graduated from generated/Lid_HG08673.cpp: the auto-generator emitted
// only kFzOnOff + kTzOnOff and a lone `state` expose, dropping the entire
// power-monitoring surface. z2m builds this with
//   tuya.modernExtend.tuyaOnOff({electricalMeasurements:true, …})
//   + tuya.modernExtend.electricityMeasurementPoll({metering:true})
// i.e. haElectricalMeasurement (rmsVoltage/rmsCurrent/activePower) plus
// seMetering (currentSummDelivered → energy, polled). Added the generic
// kFzElectricalMeasurement (→ power/voltage/current) and kFzMetering
// (→ energy/power) converters, the power/current/voltage/energy exposes,
// and the 0x0B04 (haElectricalMeasurement) + 0x0702 (seMetering) binds.
// The on/off control path is genuine for this plug and is preserved.
// z2m-source: lidl.ts #HG08673.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lidl {
namespace {
const FzConverter* const kFz_HG08673[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_HG08673[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_HG08673[] = { "TS011F" };
constexpr const char* kManus_HG08673[] = { "_TZ3000_j1v25l17", "_TZ3000_ynmowqk2", "_TZ3000_3uimvkn6" };
}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power",   ExposeType::Numeric, Access::State,    "W", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State,    "A", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State,    "V", nullptr, nullptr, 0},
    {"energy",  ExposeType::Numeric, Access::State,    "kWh", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0B04},  // haElectricalMeasurement
    {1, 0x0702},  // seMetering
};


constexpr WhiteLabel kWhiteLabels_HG08673[] = {
    {"Lidl","HG08673-BS"},
};
extern const PreparedDefinition kDef_HG08673{
    .zigbee_models=kModels_HG08673, .zigbee_models_count=sizeof(kModels_HG08673)/sizeof(kModels_HG08673[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_HG08673, .manufacturer_names_count=sizeof(kManus_HG08673)/sizeof(kManus_HG08673[0]),
    .model="HG08673", .vendor="Lidl",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_HG08673, .white_labels_count=sizeof(kWhiteLabels_HG08673)/sizeof(kWhiteLabels_HG08673[0]),
    .from_zigbee=kFz_HG08673, .from_zigbee_count=sizeof(kFz_HG08673)/sizeof(kFz_HG08673[0]),
    .to_zigbee=kTz_HG08673, .to_zigbee_count=sizeof(kTz_HG08673)/sizeof(kTz_HG08673[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lidl
