// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Innr SP 234 — hand-maintained parity override.
// Smart plug with power monitoring.
//
// Graduated from generated/Inn_SP_234.cpp: the auto-generator wired only
// kFzMetering (seMetering 0x0702 -> energy/power) and dropped the current
// and voltage channels. z2m builds this with
//   m.onOff()
//   + m.electricityMeter({current:{divisor:1000}, voltage:{divisor:1},
//                          power:{divisor:1}, energy:{divisor:100}})
// and electricityMeter() defaults to cluster:"both" /
// electricalMeasurementType:"ac", so it ALSO decodes haElectricalMeasurement
// (0x0B04 -> rmsCurrent/rmsVoltage/activePower). Added the generic
// kFzElectricalMeasurement converter (-> power/voltage/current), the
// current/voltage exposes, and the 0x0B04 bind. Mirrors the Lidl HG08673
// power-plug override.
// z2m-source: innr.ts #SP 234.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {
namespace {
const FzConverter* const kFz_SP_234[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_SP_234[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SP_234[] = { "SP 234" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0B04},  // haElectricalMeasurement
    {1, 0x0702},  // seMetering
};

extern const PreparedDefinition kDef_SP_234{
    .zigbee_models=kModels_SP_234, .zigbee_models_count=sizeof(kModels_SP_234)/sizeof(kModels_SP_234[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SP 234", .vendor="Innr",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SP_234, .from_zigbee_count=sizeof(kFz_SP_234)/sizeof(kFz_SP_234[0]),
    .to_zigbee=kTz_SP_234, .to_zigbee_count=sizeof(kTz_SP_234)/sizeof(kTz_SP_234[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::innr
