// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman HS1VS_EF — hand-maintained parity override.
//
// Graduated from generated/Hei_HS1VS_EF.cpp: the generated def lowered the
// generic kFzIasZone (which emits the bare key "alarm") while the
// expose declares the semantic key "vibration". With no rename layer the
// primary sensor state never reached the shadow. z2m decodes this via
// fz.ias_vibration_alarm_1 (zoneStatus bit 0), so the typed kFzIasVibrationAlarm converter — which emits
// "vibration" directly — is at parity. Mirrors the already-correct
// HS1CA-M / HS2WD-E heiman defs and the trust / smartthings IAS ports.
// Vibration sensor
// z2m-source: heiman.ts #HS1VS-EF.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS1VS_EF[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasVibrationAlarm,
};

constexpr const char* kModels_HS1VS_EF[] = { "Vibration-EF_3.0", "Vibration-EF-3.0" };

}  // namespace


constexpr Expose kExposes_HS1VS_EF[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"vibration", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_HS1VS_EF{
    .zigbee_models=kModels_HS1VS_EF, .zigbee_models_count=sizeof(kModels_HS1VS_EF)/sizeof(kModels_HS1VS_EF[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS1VS-EF", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HS1VS_EF, .exposes_count=sizeof(kExposes_HS1VS_EF)/sizeof(kExposes_HS1VS_EF[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS1VS_EF, .from_zigbee_count=sizeof(kFz_HS1VS_EF)/sizeof(kFz_HS1VS_EF[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
