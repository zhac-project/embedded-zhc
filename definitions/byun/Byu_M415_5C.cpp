// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: BYUN M415-5C gas sensor — graduated 2026-06-11 parity pass.
// z2m-source: byun.ts #M415-5C (Gas sensor).
//
// 2026-06-11 parity pass — DEAD-KEY + DROPPED-CHANNEL (gap classes c, b).
// The generated port wired the generic kFzIasZone, which emits the bare
// keys alarm/tamper/battery_low. But z2m drives this sensor with two
// bespoke converters that produce the single boolean expose `gas`:
//   byun_gas_true  — ssIasZone commandStatusChangeNotification, fires only
//                    when zoneStatus == 33 (0x21) → gas: true.
//   byun_gas_false — raw cluster 0x040A (1034), frame-control byte == 0x18
//                    → gas: false.
// Against kFzIasZone the alarm-CLEAR channel (0x040A) was dropped entirely
// (the sensor could latch ON forever) and the alarm key was wrong. Fixed
// by wiring the shared byun converters and exposing `gas` (boolean).

#include "definitions/byun/_shared.hpp"

namespace zhc::devices::byun {
namespace {
const FzConverter* const kFz_M415_5C[] = {
    &kFzByunGasTrue,    // ssIasZone zoneStatus==33  → gas:true
    &kFzByunGasFalse,   // raw 0x040A data[0]==0x18  → gas:false
};

constexpr const char* kModels_M415_5C[] = { "GAS  SENSOR     " };

}  // namespace


// z2m exposes: [e.gas()] — single boolean `gas`.
constexpr Expose kAutoExposes[] = {
    {"gas", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

// IAS device on endpoint 1 (alarm rides ssIasZone). Enrollment is global.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

extern const PreparedDefinition kDef_M415_5C{
    .zigbee_models=kModels_M415_5C, .zigbee_models_count=sizeof(kModels_M415_5C)/sizeof(kModels_M415_5C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="M415-5C", .vendor="Byun",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_M415_5C, .from_zigbee_count=sizeof(kFz_M415_5C)/sizeof(kFz_M415_5C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::byun
