// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0202_1 — IAS motion sensors that report motion but never
// "no motion".
// z2m-source: tuya.ts #TS0202_1 (fingerprint TS0202 + five manufacturers).
//   fromZigbee: [fz.ias_occupancy_alarm_1_with_timeout, fz.battery]
//   exposes:    occupancy, battery_low, battery, battery_voltage
//   configure:  bind genPowerCfg + batteryPercentageRemaining reporting
// z2m: "Requires alarm_1_with_timeout" (zigbee2mqtt#2818). The zone status
// decodes like the generic TS0202 (kFzIasMotionAlarm); the "no motion" half is
// the hub's: occupancy_timeout = z2m's 90 s default. Any other TS0202 keeps
// the generic def, which reports "no motion" itself.
#include "definitions/_generic/_shared.hpp"
namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasMotionAlarm,   // occupancy + tamper + battery_low
    &::zhc::generic::kFzBattery,          // battery (%) + voltage (mV)
};
constexpr const char* kModels[] = { "TS0202" };
constexpr const char* kManus[] = {
    "_TYZB01_jytabjkb", "_TZ3000_lltemgsf", "_TYZB01_5nr7ncpl", "_TZ3000_mg4dy6z6", "_TZ3000_bsvqrxru",
};
constexpr Expose kExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};
constexpr BindingSpec kBindings[] = {
    {1, 0x0001},   // genPowerCfg — battery reporting
    {1, 0x0500},   // ssIasZone — zone status notifications (as the generic TS0202)
};
}

extern const PreparedDefinition kDefTS0202_1{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus,.manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="TS0202_1",.vendor="Tuya",
    .meta=nullptr,.exposes=kExposes,.exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr,.to_zigbee_count=0,
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    // z2m fz.ias_occupancy_alarm_1_with_timeout: the hub clears occupancy 90 s after the last motion.
    .occupancy_timeout = 90,
};
}
