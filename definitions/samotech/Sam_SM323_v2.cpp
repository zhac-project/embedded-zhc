// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Samotech SM323_v2 — graduated 2026-06-10.
// Zigbee retrofit dimmer 250W
//
// PARITY FIX: z2m wires `m.electricityMeter()` with no args, which
// defaults to cluster:"both" + electricalMeasurementType:"ac" — i.e.
// seMetering (0x0702: energy) AND haElectricalMeasurement (0x0B04:
// power/voltage/current), fz=[fz.electrical_measurement, fz.metering].
// The auto-port wired only kFzMetering (0x0702) and exposed just
// state/brightness/energy/power, dropping the 0x0B04 electrical half
// (voltage + current). Added kFzElectricalMeasurement, the
// voltage/current exposes, and the 0x0B04 binding. (v1 /
// Dimmer-Switch-ZB3.0 has NO metering per z2m issue #21449 and is
// unaffected — only the SM323 / HK_DIM_A v2 carries metering.)
// z2m-source: samotech.ts #SM323_v2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::samotech {
namespace {
const FzConverter* const kFz_SM323_v2[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SM323_v2[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_SM323_v2[] = { "SM323", "HK_DIM_A" };

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

extern const PreparedDefinition kDef_SM323_v2{
    .zigbee_models=kModels_SM323_v2, .zigbee_models_count=sizeof(kModels_SM323_v2)/sizeof(kModels_SM323_v2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SM323_v2", .vendor="Samotech",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SM323_v2, .from_zigbee_count=sizeof(kFz_SM323_v2)/sizeof(kFz_SM323_v2[0]),
    .to_zigbee=kTz_SM323_v2, .to_zigbee_count=sizeof(kTz_SM323_v2)/sizeof(kTz_SM323_v2[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::samotech
