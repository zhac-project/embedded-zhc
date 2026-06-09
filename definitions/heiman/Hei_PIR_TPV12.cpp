// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman PIR_TPV12 — hand-maintained parity override.
//
// Graduated from generated/Hei_PIR_TPV12.cpp: the generated def lowered the
// generic kFzIasZone (which emits the bare key "alarm") while the
// expose declares the semantic key "occupancy". With no rename layer the
// primary sensor state never reached the shadow. z2m decodes this via
// fz.ias_occupancy_alarm_1 (zoneStatus bit 0), so the typed kFzIasMotionAlarm converter — which emits
// "occupancy" directly — is at parity. Mirrors the already-correct
// HS1CA-M / HS2WD-E heiman defs and the trust / smartthings IAS ports.
// Motion sensor
// z2m-source: heiman.ts #PIR_TPV12.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_PIR_TPV12[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_PIR_TPV12[] = { "PIR_TPV12" };

}  // namespace


constexpr Expose kExposes_PIR_TPV12[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_PIR_TPV12{
    .zigbee_models=kModels_PIR_TPV12, .zigbee_models_count=sizeof(kModels_PIR_TPV12)/sizeof(kModels_PIR_TPV12[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PIR_TPV12", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_PIR_TPV12, .exposes_count=sizeof(kExposes_PIR_TPV12)/sizeof(kExposes_PIR_TPV12[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PIR_TPV12, .from_zigbee_count=sizeof(kFz_PIR_TPV12)/sizeof(kFz_PIR_TPV12[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
