// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Samotech SM309-S — graduated 2026-06-10.
// Zigbee dimmer 400W with power and energy metering
//
// PARITY FIX: z2m wires `m.electricityMeter()` with no args, which
// defaults to cluster:"both" + electricalMeasurementType:"ac" — i.e.
// seMetering (0x0702: energy) AND haElectricalMeasurement (0x0B04:
// power/voltage/current), fz=[fz.electrical_measurement, fz.metering].
// The auto-port wired only kFzMetering (0x0702) and exposed just
// state/brightness/energy/power, dropping the 0x0B04 electrical half
// (voltage + current). Added kFzElectricalMeasurement, the
// voltage/current exposes, and the 0x0B04 binding.
// z2m-source: samotech.ts #SM309-S.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::samotech {
namespace {
const FzConverter* const kFz_SM309_S[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SM309_S[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_SM309_S[] = { "SM309-S" };

}  // namespace


// --- hand-edited exposes/bindings (electricityMeter cluster:"both") ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end hand-edited block ---

extern const PreparedDefinition kDef_SM309_S{
    .zigbee_models=kModels_SM309_S, .zigbee_models_count=sizeof(kModels_SM309_S)/sizeof(kModels_SM309_S[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SM309-S", .vendor="Samotech",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SM309_S, .from_zigbee_count=sizeof(kFz_SM309_S)/sizeof(kFz_SM309_S[0]),
    .to_zigbee=kTz_SM309_S, .to_zigbee_count=sizeof(kTz_SM309_S)/sizeof(kTz_SM309_S[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::samotech
