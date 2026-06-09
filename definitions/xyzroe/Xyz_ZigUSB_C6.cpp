// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Xyzroe ZigUSB_C6 — graduated from generated/ for parity fix.
// Zigbee USB switch with monitoring (4 endpoints).
// z2m-source: xyzroe.ts #ZigUSB_C6.
//
// z2m builds this device entirely from modernExtend:
//   m.electricityMeter({cluster:"electrical", electricalMeasurementType:"both", ...})
//   m.temperature(), m.onOff() x3, m.iasZoneAlarm({zoneType:"generic", zoneAttributes:["alarm_1"]})
//
// Parity notes (vs the prior hand-rewrite that wired kFzMetering + an
// `energy` expose):
//   * m.electricityMeter with cluster:"electrical" maps ONLY to
//     fz.electrical_measurement (haElectricalMeasurement 0x0B04) and
//     exposes power/voltage/current — NO seMetering, NO `energy`. The
//     earlier kFzMetering + `energy` + {1,0x0702} binding were phantom
//     and are removed here.
//   * m.iasZoneAlarm (over-current alarm) was dropped in the prior
//     rewrite; it is a real ssIasZone (0x0500) alarm_1 channel and is
//     restored via kFzIasZoneStatusChange (bit0 -> alarm_1, non-inverted
//     for zoneType "generic"). IAS enrollment is handled globally.
//
// Endpoints (from m.deviceEndpoints {1,2,3,4}, endpoint_map labels 1/2/3):
//   1: USB-port switch (state_1) + electrical metering (power/voltage/current)
//      + over-current alarm
//   2: Zigbee-status LED switch (state_2)
//   3: USB-state LED switch (state_3) + device temperature
//   4: restart timer (SET-only, custom tz)
//
// Runtime key shape: with endpoint_map present the dispatch layer suffixes
// emitted keys `<key>_<label>`, EXCEPT keys in dispatch.cpp kAlwaysGlobalKeys
// (voltage, tamper, …) which stay bare. So the standard converters here
// surface as: power_1 / current_1 / voltage (forced-global) / temperature_3 /
// state_1 / state_2 / state_3 / alarm_1_1 (IAS fired on ep1). The exposes
// below are named to match that runtime output so every channel surfaces.
// (z2m names voltage `voltage_1`, temperature bare, alarm `alarm_1`; the
// suffix divergences are framework invariants, not modelling choices.)
//
// INFRA / deferred: the SET-only `restart` (z2m tzLocal.zigusb_restart_interval,
// a vendor genOnOff OffWaitTime-style command on ep4) has no generic tz
// converter — kept as a placeholder SET expose, non-functional until a
// vendor tz is added.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::xyzroe {
namespace {
const FzConverter* const kFz_ZigUSB_C6[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasZoneStatusChange,  // over-current alarm -> alarm_1
};
const TzConverter* const kTz_ZigUSB_C6[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_ZigUSB_C6[] = { "ZigUSB_C6" };

constexpr Expose kExp_ZigUSB_C6[] = {
    // ep1 — USB-port switch + electrical metering + over-current alarm
    {"state_1",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,       nullptr, nullptr, 0},
    {"power_1",       ExposeType::Numeric, ::zhc::Access::State,    "W",           nullptr, nullptr, 0},
    {"voltage",       ExposeType::Numeric, ::zhc::Access::State,    "V",           nullptr, nullptr, 0},  // forced-global key
    {"current_1",     ExposeType::Numeric, ::zhc::Access::State,    "A",           nullptr, nullptr, 0},
    {"alarm_1_1",     ExposeType::Binary,  ::zhc::Access::State,    nullptr,       nullptr, nullptr, 0},  // over-current (ssIasZone ep1)
    // ep2 / ep3 — indicator-LED switches
    {"state_2",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,       nullptr, nullptr, 0},
    {"state_3",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,       nullptr, nullptr, 0},
    // ep3 — device temperature
    {"temperature_3", ExposeType::Numeric, ::zhc::Access::State,    "\xc2\xb0""C", nullptr, nullptr, 0},
    // ep4 — restart timer (SET-only, custom vendor tz; non-functional placeholder)
    {"restart",       ExposeType::Numeric, ::zhc::Access::Set,      "s",           nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_ZigUSB_C6[] = {
    {1, 0x0006},  // genOnOff (ep1 USB-port switch)  — m.onOff endpointNames ["1"]
    {2, 0x0006},  // genOnOff (ep2 Zigbee-status LED) — m.onOff endpointNames ["2"]
    {3, 0x0006},  // genOnOff (ep3 USB-state LED)     — m.onOff endpointNames ["3"]
    {1, 0x0B04},  // haElectricalMeasurement (power/voltage/current)
    {1, 0x0500},  // ssIasZone (over-current alarm)
    {3, 0x0402},  // msTemperatureMeasurement (device temperature)
};
constexpr ::zhc::EndpointLabel kEndpoints_ZigUSB_C6[] = { {"1", 1}, {"2", 2}, {"3", 3} };

}  // namespace

extern const PreparedDefinition kDef_ZigUSB_C6{
    .zigbee_models=kModels_ZigUSB_C6, .zigbee_models_count=sizeof(kModels_ZigUSB_C6)/sizeof(kModels_ZigUSB_C6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZigUSB_C6", .vendor="Xyzroe",
    .meta=nullptr, .exposes=kExp_ZigUSB_C6, .exposes_count=sizeof(kExp_ZigUSB_C6)/sizeof(kExp_ZigUSB_C6[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZigUSB_C6, .from_zigbee_count=sizeof(kFz_ZigUSB_C6)/sizeof(kFz_ZigUSB_C6[0]),
    .to_zigbee=kTz_ZigUSB_C6, .to_zigbee_count=sizeof(kTz_ZigUSB_C6)/sizeof(kTz_ZigUSB_C6[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_ZigUSB_C6, .bindings_count=sizeof(kBind_ZigUSB_C6)/sizeof(kBind_ZigUSB_C6[0]),
    .endpoint_map       = kEndpoints_ZigUSB_C6,
    .endpoint_map_count = sizeof(kEndpoints_ZigUSB_C6)/sizeof(kEndpoints_ZigUSB_C6[0]),
};

}  // namespace zhc::devices::xyzroe
