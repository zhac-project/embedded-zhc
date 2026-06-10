// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HzcElectric S902M-ZG — graduated from generated/.
// Motion sensor
// z2m-source: hzc_electric.ts #S902M-ZG.
//
// Two bugs fixed:
//  (1) IAS dead-key: the generated port lowered the generic kFzIasZone
//      converter (bare key `alarm`) while z2m's fz.ias_occupancy_alarm_1
//      publishes `occupancy` (zoneStatus bit 0). Re-pointed at the typed
//      kFzIasMotionAlarm (bit 0 -> occupancy, bit 2 -> tamper, bit 3 ->
//      battery_low) and renamed the expose alarm -> occupancy.
//  (2) Dropped illuminance channel: z2m adds m.illuminance()
//      (msIlluminanceMeasurement 0x0400). The port omitted it entirely —
//      restored the generic kFzIlluminance + the illuminance expose +
//      0x0400 binding. Battery path (fz.battery) is unchanged.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hzc_electric {
namespace {
const FzConverter* const kFz_S902M_ZG[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_S902M_ZG[] = { "HZC Electric motion sensor" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0400},
};

extern const PreparedDefinition kDef_S902M_ZG{
    .zigbee_models=kModels_S902M_ZG, .zigbee_models_count=sizeof(kModels_S902M_ZG)/sizeof(kModels_S902M_ZG[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S902M-ZG", .vendor="HzcElectric",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S902M_ZG, .from_zigbee_count=sizeof(kFz_S902M_ZG)/sizeof(kFz_S902M_ZG[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hzc_electric
