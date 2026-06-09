// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman HS3MS — hand-maintained parity override.
//
// Graduated from generated/Hei_HS3MS.cpp: the generated def lowered the
// generic kFzIasZone (which emits the bare key "alarm") while the
// expose declares the semantic key "occupancy". With no rename layer the
// primary sensor state never reached the shadow. z2m decodes this via
// fz.ias_occupancy_alarm_1 (zoneStatus bit 0), so the typed kFzIasMotionAlarm converter — which emits
// "occupancy" directly — is at parity. Mirrors the already-correct
// HS1CA-M / HS2WD-E heiman defs and the trust / smartthings IAS ports.
// Smart motion sensor
// z2m-source: heiman.ts #HS3MS.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS3MS[] = {
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_HS3MS[] = { "PIRSensor-N", "PIRSensor-N-3.0", "PIRSensor-EM", "PIRSensor-EF-3.0", "PIR_TPV13" };

}  // namespace


constexpr Expose kExposes_HS3MS[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

extern const PreparedDefinition kDef_HS3MS{
    .zigbee_models=kModels_HS3MS, .zigbee_models_count=sizeof(kModels_HS3MS)/sizeof(kModels_HS3MS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS3MS", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HS3MS, .exposes_count=sizeof(kExposes_HS3MS)/sizeof(kExposes_HS3MS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS3MS, .from_zigbee_count=sizeof(kFz_HS3MS)/sizeof(kFz_HS3MS[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
