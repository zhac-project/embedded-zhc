// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HzcElectric S900W-ZG — graduated from generated/.
// Water leak sensor
// z2m-source: hzc_electric.ts #S900W-ZG.
//
// Bug fixed (IAS dead-key): the generated port lowered the generic
// kFzIasZone converter, which emits the bare key `alarm`, while z2m's
// fz.ias_water_leak_alarm_1 publishes the semantic key `water_leak`
// (zoneStatus bit 0). The water-leak state never reached the shadow.
// Re-pointed at the typed kFzIasWaterLeakAlarm (bit 0 -> water_leak,
// bit 2 -> tamper, bit 3 -> battery_low) and renamed the expose
// alarm -> water_leak. Battery path (fz.battery) is unchanged.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hzc_electric {
namespace {
const FzConverter* const kFz_S900W_ZG[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_S900W_ZG[] = { "WaterLeakageSensor-ZB3.0" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_S900W_ZG{
    .zigbee_models=kModels_S900W_ZG, .zigbee_models_count=sizeof(kModels_S900W_ZG)/sizeof(kModels_S900W_ZG[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S900W-ZG", .vendor="HzcElectric",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S900W_ZG, .from_zigbee_count=sizeof(kFz_S900W_ZG)/sizeof(kFz_S900W_ZG[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hzc_electric
