// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Casaia CTHS-317-ET — remote temperature probe on cable.
//
// The auto-generated port (Tier 1) reduced this device to battery-only: it
// wired only the generic kFzBattery and exposed battery/voltage. But z2m's
// CTHS-317-ET declares `fromZigbee: [fz.temperature, fz.battery]` and
// `exposes: [e.temperature(), e.battery_low(), e.battery()]` — the entire
// reason this device exists (the temperature probe) was dropped on decode,
// and the battery_low alarm expose was missing too.
//
// Fix: add the generic kFzTemperature decoder (msTemperatureMeasurement
// 0x0402, /100) + the temperature & battery_low exposes. kFzBattery already
// emits battery_low from batteryAlarmState (attr 0x0035), matching z2m.
// z2m binds both clusters on endpoint 3, so the bindings target ep3.
//
// z2m-source: casaia.ts #CTHS-317-ET.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::casaia {
namespace {
const FzConverter* const kFz_CTHS_317_ET[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_CTHS_317_ET[] = { "CTHS317ET" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {3, 0x0402},  // msTemperatureMeasurement
    {3, 0x0001},  // genPowerCfg
};

extern const PreparedDefinition kDef_CTHS_317_ET{
    .zigbee_models=kModels_CTHS_317_ET, .zigbee_models_count=sizeof(kModels_CTHS_317_ET)/sizeof(kModels_CTHS_317_ET[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CTHS-317-ET", .vendor="Casaia",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CTHS_317_ET, .from_zigbee_count=sizeof(kFz_CTHS_317_ET)/sizeof(kFz_CTHS_317_ET[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::casaia
