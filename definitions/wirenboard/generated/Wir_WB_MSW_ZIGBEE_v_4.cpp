// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Wirenboard WB-MSW-ZIGBEE v.4 — hand-rewritten from z2m source.
// Wall-mounted multi sensor — modernExtend variant of v.3 that adds
// illuminance + an activity-indicator endpoint and grows the manuSpec
// attribute set on `genBasic` and `sprutDevice`.
// z2m-source: wirenboard.ts #WB-MSW-ZIGBEE v.4.
//
// Endpoints:
//   l1, l2, l3   — three on/off relays (cluster 0x0006).
//   indicator    — activity LED switch (cluster 0x0006 + manuSpec attr).
//
// Standard clusters reported on:
//   msTemperatureMeasurement 0x0402  (sprutTemperatureOffset 0x6600 manu)
//   msRelativeHumidity       0x0405  (sprutHeater 0x6600 manu)
//   msOccupancySensing       0x0406  (sprutOccupancyLevel/Sensitivity manu)
//   msCO2                    0x040D  (sprutCO2Calibration/Auto manu)
//   msIlluminanceMeasurement 0x0400  (no manu attrs, pure standard)
//
// Custom clusters: sprutVoc 0x6601, sprutNoise 0x6602,
//                  sprutIrBlaster 0x6603, sprutDevice 0x6680.
//
// Same shadow-only stance as v.3 for the manuSpec writes — see comment
// in Wir_WB_MSW_ZIGBEE_v_3.cpp. A wirenboard/_shared.{hpp,cpp} bundle
// would let us land the TZ writes for both ports together; deferred
// because the entire vendor only contains 2 devices.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::wirenboard {
namespace {

const FzConverter* const kFz_WB_MSW_ZIGBEE_v_4[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
};
const TzConverter* const kTz_WB_MSW_ZIGBEE_v_4[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_WB_MSW_ZIGBEE_v_4[] = { "WBMSW4" };

constexpr Expose kExposes_WB_MSW_ZIGBEE_v_4[] = {
    // Sensors (live state).
    { "temperature",       ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",          ExposeType::Numeric, Access::State,    "%",           nullptr, nullptr, 0 },
    { "illuminance",       ExposeType::Numeric, Access::State,    "lx",          nullptr, nullptr, 0 },
    { "occupancy",         ExposeType::Binary,  Access::State,    nullptr,       nullptr, nullptr, 0 },
    { "occupancy_level",   ExposeType::Numeric, Access::State,    nullptr,       nullptr, nullptr, 0,
      ExposeCategory::Diagnostic },
    { "co2",               ExposeType::Numeric, Access::State,    "ppm",         nullptr, nullptr, 0 },
    { "voc",               ExposeType::Numeric, Access::State,    "ppb",         nullptr, nullptr, 0 },
    { "noise",             ExposeType::Numeric, Access::State,    "dBA",         nullptr, nullptr, 0 },
    { "noise_detected",    ExposeType::Binary,  Access::State,    nullptr,       nullptr, nullptr, 0 },

    // Relays (l1/l2/l3) + activity-LED indicator endpoint.
    { "state_l1",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l2",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l3",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_indicator",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },

    // Diagnostic — sprutDevice manuSpec attrs.
    { "is_connected",      ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0,
      ExposeCategory::Diagnostic },

    // Config — manuSpec writes (TZ deferred; shadow accepts pushes).
    { "noise_timeout",          ExposeType::Numeric, Access::State, "s",   nullptr, nullptr, 0,
      ExposeCategory::Config },
    { "occupancy_timeout",      ExposeType::Numeric, Access::State, "s",   nullptr, nullptr, 0,
      ExposeCategory::Config },
    { "temperature_offset",     ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0,
      ExposeCategory::Config },
    { "occupancy_sensitivity",  ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0,
      ExposeCategory::Config },
    { "noise_detect_level",     ExposeType::Numeric, Access::State, "dBA", nullptr, nullptr, 0,
      ExposeCategory::Config },
    { "co2_autocalibration",    ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0,
      ExposeCategory::Config },
    { "co2_manual_calibration", ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0,
      ExposeCategory::Config },
    { "th_heater",              ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0,
      ExposeCategory::Config },
    { "uart_baud_rate",         ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0,
      ExposeCategory::Config },
};

constexpr BindingSpec kBindings_WB_MSW_ZIGBEE_v_4[] = {
    // Relay endpoints + indicator.
    { 1, 0x0006 },
    { 2, 0x0006 },
    { 3, 0x0006 },
    { 4, 0x0006 },
    // Sensor clusters (single endpoint).
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
    { 1, 0x0400 },  // msIlluminanceMeasurement (new in v.4)
    { 1, 0x0406 },  // msOccupancySensing
    { 1, 0x040D },  // msCO2
    { 1, 0x6601 },  // sprutVoc
    { 1, 0x6602 },  // sprutNoise
    { 1, 0x6680 },  // sprutDevice (isConnected, UartBaudRate)
};

constexpr ::zhc::EndpointLabel kEndpoints_WB_MSW_ZIGBEE_v_4[] = { {"default", 1}, {"l1", 2}, {"l2", 3}, {"l3", 4}, {"indicator", 5} };

}  // namespace

extern const PreparedDefinition kDef_WB_MSW_ZIGBEE_v_4{
    .zigbee_models=kModels_WB_MSW_ZIGBEE_v_4, .zigbee_models_count=sizeof(kModels_WB_MSW_ZIGBEE_v_4)/sizeof(kModels_WB_MSW_ZIGBEE_v_4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WB-MSW-ZIGBEE v.4", .vendor="Wirenboard",
    .meta=nullptr, .exposes=kExposes_WB_MSW_ZIGBEE_v_4,
    .exposes_count=sizeof(kExposes_WB_MSW_ZIGBEE_v_4)/sizeof(kExposes_WB_MSW_ZIGBEE_v_4[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WB_MSW_ZIGBEE_v_4, .from_zigbee_count=sizeof(kFz_WB_MSW_ZIGBEE_v_4)/sizeof(kFz_WB_MSW_ZIGBEE_v_4[0]),
    .to_zigbee=kTz_WB_MSW_ZIGBEE_v_4, .to_zigbee_count=sizeof(kTz_WB_MSW_ZIGBEE_v_4)/sizeof(kTz_WB_MSW_ZIGBEE_v_4[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_WB_MSW_ZIGBEE_v_4,
    .bindings_count=sizeof(kBindings_WB_MSW_ZIGBEE_v_4)/sizeof(kBindings_WB_MSW_ZIGBEE_v_4[0]),
    .endpoint_map       = kEndpoints_WB_MSW_ZIGBEE_v_4,
    .endpoint_map_count = sizeof(kEndpoints_WB_MSW_ZIGBEE_v_4)/sizeof(kEndpoints_WB_MSW_ZIGBEE_v_4[0]),
};

}  // namespace zhc::devices::wirenboard
