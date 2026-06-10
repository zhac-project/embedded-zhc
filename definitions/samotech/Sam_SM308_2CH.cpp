// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Samotech SM308-2CH — graduated 2026-06-10.
// Zigbee 2 channel in wall switch (per-channel relay state_l1/state_l2,
// whole-device metering on EP1).
//
// PARITY FIX: z2m wires `m.electricityMeter()` with no args, which
// defaults to cluster:"both" + electricalMeasurementType:"ac" — i.e.
// seMetering (0x0702: energy) AND haElectricalMeasurement (0x0B04:
// power/voltage/current), fz=[fz.electrical_measurement, fz.metering].
// The auto-port wired only kFzMetering (0x0702) and exposed just
// state/energy/power, dropping the 0x0B04 electrical half (voltage +
// current). Added kFzElectricalMeasurement, the voltage/current
// exposes, and the 0x0B04 binding.
//
// The endpoint_map {l1:1, l2:2} is CORRECT and retained: the SUSPECTED
// missing-endpoint_map is a false flag — the relay on/off state is
// genuinely per-channel (z2m m.onOff({endpointNames:["l1","l2"]}) ->
// state_l1 / state_l2). z2m's meta.multiEndpointSkip keeps metering
// whole-device; the metering frame reports from EP1 only.
// z2m-source: samotech.ts #SM308-2CH.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::samotech {
namespace {
const FzConverter* const kFz_SM308_2CH[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SM308_2CH[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SM308_2CH[] = { "SM308-2CH" };

constexpr ::zhc::EndpointLabel kEndpoints_SM308_2CH[] = { {"l1", 1}, {"l2", 2} };

}  // namespace


// --- hand-edited exposes/bindings (electricityMeter cluster:"both") ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end hand-edited block ---

extern const PreparedDefinition kDef_SM308_2CH{
    .zigbee_models=kModels_SM308_2CH, .zigbee_models_count=sizeof(kModels_SM308_2CH)/sizeof(kModels_SM308_2CH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SM308-2CH", .vendor="Samotech",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SM308_2CH, .from_zigbee_count=sizeof(kFz_SM308_2CH)/sizeof(kFz_SM308_2CH[0]),
    .to_zigbee=kTz_SM308_2CH, .to_zigbee_count=sizeof(kTz_SM308_2CH)/sizeof(kTz_SM308_2CH[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_SM308_2CH,
    .endpoint_map_count = sizeof(kEndpoints_SM308_2CH)/sizeof(kEndpoints_SM308_2CH[0]),
};

}  // namespace zhc::devices::samotech
