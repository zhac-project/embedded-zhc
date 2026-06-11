// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: BYUN M415-6C smoke sensor — graduated 2026-06-11 parity pass.
// z2m-source: byun.ts #M415-6C (Smoke sensor; zigbeeModel "Windows switch  ").
//
// 2026-06-11 parity pass — DEAD-KEY + DROPPED-CHANNEL (gap classes c, b).
// The generated port wired the generic kFzIasZone, which emits the bare
// keys alarm/tamper/battery_low. But z2m drives this sensor with two
// bespoke converters that produce the single boolean expose `smoke`:
//   byun_smoke_true  — ssIasZone commandStatusChangeNotification, fires only
//                      when zoneStatus == 33 (0x21) → smoke: true.
//   byun_smoke_false — pHMeasurement (0x0409) attributeReport, measuredValue
//                      == 0 → smoke: false.
// Against kFzIasZone the alarm-CLEAR channel (0x0409) was dropped entirely
// (the sensor could latch ON forever) and the alarm key was wrong. Fixed
// by wiring the shared byun converters and exposing `smoke` (boolean).

#include "definitions/byun/_shared.hpp"

namespace zhc::devices::byun {
namespace {
const FzConverter* const kFz_M415_6C[] = {
    &kFzByunSmokeTrue,   // ssIasZone zoneStatus==33      → smoke:true
    &kFzByunSmokeFalse,  // pHMeasurement measuredValue==0 → smoke:false
};

constexpr const char* kModels_M415_6C[] = { "Windows switch  " };

}  // namespace


// z2m exposes: [e.smoke()] — single boolean `smoke`.
constexpr Expose kAutoExposes[] = {
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

// IAS device on endpoint 1 (alarm rides ssIasZone). Enrollment is global.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

extern const PreparedDefinition kDef_M415_6C{
    .zigbee_models=kModels_M415_6C, .zigbee_models_count=sizeof(kModels_M415_6C)/sizeof(kModels_M415_6C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="M415-6C", .vendor="Byun",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_M415_6C, .from_zigbee_count=sizeof(kFz_M415_6C)/sizeof(kFz_M415_6C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::byun
