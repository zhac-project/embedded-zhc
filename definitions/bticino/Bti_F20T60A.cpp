// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bticino F20T60A — DIN power module measures via haElectricalMeasurement, not seMetering.
//
// The generated def wired kFzMetering (seMetering 0x0702) and exposed
// energy/power. But z2m uses m.electricityMeter({cluster: "electrical"})
// which is haElectricalMeasurement-ONLY (fz.electrical_measurement,
// cluster 0x0B04): it exposes power/voltage/current and NO energy
// (electricityMeter defaults acFrequency/powerFactor to false). The
// ported def therefore (a) bound + decoded the wrong cluster, (b)
// surfaced a phantom `energy` key that never arrives, and (c) dropped
// voltage + current entirely. Re-wired to kFzElectricalMeasurement
// (0x0B04) with power/voltage/current exposes to match z2m exactly.
//
// z2m-source: bticino.ts #F20T60A —
//   extend: [legrandExtend.addLegrandDevicesCluster(), m.onOff(),
//            m.electricityMeter({cluster: "electrical"})]
//   (cluster "electrical" => fz.electrical_measurement, exposes
//    power/voltage/current)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bticino {
namespace {
const FzConverter* const kFz_F20T60A[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_F20T60A[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_F20T60A[] = { "Bticino Din power consumption module " };

}  // namespace


// --- exposes/bindings: power/voltage/current via haElectricalMeasurement ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0B04},
};
// --- end block ---

extern const PreparedDefinition kDef_F20T60A{
    .zigbee_models=kModels_F20T60A, .zigbee_models_count=sizeof(kModels_F20T60A)/sizeof(kModels_F20T60A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="F20T60A", .vendor="Bticino",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_F20T60A, .from_zigbee_count=sizeof(kFz_F20T60A)/sizeof(kFz_F20T60A[0]),
    .to_zigbee=kTz_F20T60A, .to_zigbee_count=sizeof(kTz_F20T60A)/sizeof(kTz_F20T60A[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bticino
