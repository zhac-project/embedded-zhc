// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: graduated to fix IAS dead-key + missing temperature.
// Calypso smart water-heater controller (with leak detection).
//
// z2m-source: sinope.ts #RM3500ZB — fromZigbee
//   [fzLocal.ias_water_leak_alarm, fzLocal.sinope, fz.temperature].
//   fzLocal.ias_water_leak_alarm emits {water_leak, tamper} from
//   ssIasZone zoneStatus bits 0 / 2.
//
// The generated def lowered the generic kFzIasZone (bare "alarm" key)
// behind an "alarm" expose, so the leak state never reached the shadow,
// and it omitted the on-board temperature sensor. Swap in the typed
// kFzIasWaterLeakAlarm (emits water_leak) and add kFzTemperature +
// the msTemperatureMeasurement (0x0402) bind. The relay (genOnOff) +
// energy metering (seMetering) paths are retained.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sinope {
namespace {
const FzConverter* const kFz_RM3500ZB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};
const TzConverter* const kTz_RM3500ZB[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_RM3500ZB[] = { "RM3500ZB" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0702},  // seMetering
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_RM3500ZB{
    .zigbee_models=kModels_RM3500ZB, .zigbee_models_count=sizeof(kModels_RM3500ZB)/sizeof(kModels_RM3500ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RM3500ZB", .vendor="Sinope",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RM3500ZB, .from_zigbee_count=sizeof(kFz_RM3500ZB)/sizeof(kFz_RM3500ZB[0]),
    .to_zigbee=kTz_RM3500ZB, .to_zigbee_count=sizeof(kTz_RM3500ZB)/sizeof(kTz_RM3500ZB[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sinope
