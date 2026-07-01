// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Wirenboard WB-MSW-ZIGBEE v.4 (official firmware) multi sensor (z2m v26.76.0 parity).
// z2m-source: wirenboard.ts #WB-MSW-ZIGBEE_v.4_official
//             + converters/fromZigbee.ts fz.occupancy / fz.co2.
//
// New device (z2m v26.76.0). Distinct fingerprint from the existing
// #WB-MSW-ZIGBEE v.4 (zigbeeModel "WBMSW4", sprutDevice-based): the
// "official Wiren Board firmware" build reports zigbeeModel
// "WB-MSW-ZIGBEE v.4" over STANDARD clusters plus a few vendor-custom
// ones. Same porting stance as its sibling Wir_WB_MSW_ZIGBEE_v_4.cpp:
// the primary multi-sensor readings live on standard clusters and get
// real generic decoders; the vendor-custom noise / VOC / device-info
// channels have no generic decoder and are declared shadow-only
// (exposed, TZ deferred — shadow accepts pushes).
//
// Endpoints (m.deviceEndpoints, multiEndpointSkip:["occupancy"]):
//   default 1  — all sensors report here.
//   buzzer 2, heater 3, led_red 4, led_green 5 — on/off (cluster 0x0006).
//
// Decoded (standard clusters, generic converters):
//   msTemperatureMeasurement 0x0402 -> temperature   (kFzTemperature)
//   msRelativeHumidity       0x0405 -> humidity       (kFzHumidity)
//   msIlluminanceMeasurement 0x0400 -> illuminance    (kFzIlluminance)
//   msOccupancySensing       0x0406 -> occupancy      (kFzOccupancy)
//   msCO2                    0x040D -> co2            (kFzCO2)
//   genOnOff                 0x0006 -> state_* relays (kFzOnOff/kTzOnOff)
//
// Shadow-only (vendor-custom clusters, no generic decoder):
//   genAnalogInput   0x000C — noise_level / noise / noise_threshold / noise_timeout
//   genBinaryOutput  0x0010 — status_led
//   genMultistateInput 0x0012 — connectivity + device-info (slave id, serial, fw/boot/component)
//   msTemperatureMeasurement.temperatureOffset (manu attr) — temperature_offset
//   msOccupancySensing.occupancy{Level,Sensitivity,Timeout} (manu attrs)
//   wbVoc            0x042e — voc
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::wirenboard {
namespace {

const FzConverter* const kFz_WB_MSW_ZIGBEE_v_4_official[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzCO2,
};
const TzConverter* const kTz_WB_MSW_ZIGBEE_v_4_official[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_WB_MSW_ZIGBEE_v_4_official[] = { "WB-MSW-ZIGBEE v.4" };

constexpr Expose kExposes_WB_MSW_ZIGBEE_v_4_official[] = {
    // Sensors (live state, decoded).
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",           nullptr, nullptr, 0 },
    { "illuminance", ExposeType::Numeric, Access::State, "lx",          nullptr, nullptr, 0 },
    { "occupancy",   ExposeType::Binary,  Access::State, nullptr,       nullptr, nullptr, 0 },
    { "co2",         ExposeType::Numeric, Access::State, "ppm",         nullptr, nullptr, 0 },

    // Relays (buzzer / heater / led_red / led_green) — genOnOff, StateSet.
    { "state_buzzer",    ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_heater",    ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_led_red",   ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_led_green", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0 },

    // Vendor-custom noise channel (genAnalogInput) — shadow-only.
    { "noise_level",     ExposeType::Numeric, Access::State, "dBA", "Current noise level", nullptr, 0 },
    { "noise",           ExposeType::Binary,  Access::State, nullptr, "Noise detected", nullptr, 0 },
    { "noise_threshold", ExposeType::Numeric, Access::State, "dBA", "Noise detection threshold", nullptr, 0,
      ExposeCategory::Config },
    { "noise_timeout",   ExposeType::Numeric, Access::State, "s", "Time in seconds after which noise is cleared", nullptr, 0,
      ExposeCategory::Config },

    // Status LED (genBinaryOutput) — shadow-only (TZ deferred).
    { "status_led",      ExposeType::Binary,  Access::State, nullptr, "Status LED control", nullptr, 0,
      ExposeCategory::Config },

    // Device info (genMultistateInput manu attrs) — diagnostics, shadow-only.
    { "connectivity",       ExposeType::Enum,    Access::State, nullptr, "Device connectivity state", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "modbus_slave_id",    ExposeType::Numeric, Access::State, nullptr, "Device Modbus slave ID", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "serial_number",      ExposeType::Numeric, Access::State, nullptr, "Device serial number", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "fw_version",         ExposeType::String,  Access::State, nullptr, "Device firmware version", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "fw_signature",       ExposeType::String,  Access::State, nullptr, "Device firmware signature", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "boot_version",       ExposeType::String,  Access::State, nullptr, "Device bootloader version", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "component_version",  ExposeType::String,  Access::State, nullptr, "Device component firmware version", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "component_signature",ExposeType::String,  Access::State, nullptr, "Device component firmware signature", nullptr, 0,
      ExposeCategory::Diagnostic },

    // Temperature offset (msTemperatureMeasurement manu attr) — shadow-only.
    { "temperature_offset", ExposeType::Numeric, Access::State, "\xC2\xB0""C", "Offset subtracted from the raw temperature reading", nullptr, 0,
      ExposeCategory::Config },

    // Occupancy tuning (msOccupancySensing manu attrs) — shadow-only.
    { "occupancy_level",       ExposeType::Numeric, Access::State, nullptr, "Raw occupancy level reported by the sensor", nullptr, 0,
      ExposeCategory::Diagnostic },
    { "occupancy_sensitivity", ExposeType::Numeric, Access::State, nullptr, "Occupancy detection sensitivity", nullptr, 0,
      ExposeCategory::Config },
    { "occupancy_timeout",     ExposeType::Numeric, Access::State, "s", "Time in seconds after which occupancy is cleared", nullptr, 0,
      ExposeCategory::Config },

    // VOC (wbVoc custom cluster 0x042e) — shadow-only.
    { "voc", ExposeType::Numeric, Access::State, "\xC2\xB5""g/m\xC2\xB3", "Measured VOC concentration", nullptr, 0 },
};

constexpr BindingSpec kBindings_WB_MSW_ZIGBEE_v_4_official[] = {
    // On/off relay endpoints.
    { 2, 0x0006 },  // buzzer
    { 3, 0x0006 },  // heater
    { 4, 0x0006 },  // led_red
    { 5, 0x0006 },  // led_green
    // Sensor / custom clusters (single endpoint, firmware-driven reporting).
    { 1, 0x0400 },  // msIlluminanceMeasurement
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
    { 1, 0x0406 },  // msOccupancySensing
    { 1, 0x040D },  // msCO2
    { 1, 0x000C },  // genAnalogInput (noise)
    { 1, 0x0010 },  // genBinaryOutput (status_led)
    { 1, 0x0012 },  // genMultistateInput (device info)
    { 1, 0x042E },  // wbVoc
};

constexpr ::zhc::EndpointLabel kEndpoints_WB_MSW_ZIGBEE_v_4_official[] = {
    {"default", 1}, {"buzzer", 2}, {"heater", 3}, {"led_red", 4}, {"led_green", 5}
};

}  // namespace

extern const PreparedDefinition kDef_WB_MSW_ZIGBEE_v_4_official{
    .zigbee_models=kModels_WB_MSW_ZIGBEE_v_4_official, .zigbee_models_count=sizeof(kModels_WB_MSW_ZIGBEE_v_4_official)/sizeof(kModels_WB_MSW_ZIGBEE_v_4_official[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WB-MSW-ZIGBEE_v.4_official", .vendor="Wirenboard",
    .meta=nullptr, .exposes=kExposes_WB_MSW_ZIGBEE_v_4_official,
    .exposes_count=sizeof(kExposes_WB_MSW_ZIGBEE_v_4_official)/sizeof(kExposes_WB_MSW_ZIGBEE_v_4_official[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WB_MSW_ZIGBEE_v_4_official, .from_zigbee_count=sizeof(kFz_WB_MSW_ZIGBEE_v_4_official)/sizeof(kFz_WB_MSW_ZIGBEE_v_4_official[0]),
    .to_zigbee=kTz_WB_MSW_ZIGBEE_v_4_official, .to_zigbee_count=sizeof(kTz_WB_MSW_ZIGBEE_v_4_official)/sizeof(kTz_WB_MSW_ZIGBEE_v_4_official[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_WB_MSW_ZIGBEE_v_4_official,
    .bindings_count=sizeof(kBindings_WB_MSW_ZIGBEE_v_4_official)/sizeof(kBindings_WB_MSW_ZIGBEE_v_4_official[0]),
    .endpoint_map       = kEndpoints_WB_MSW_ZIGBEE_v_4_official,
    .endpoint_map_count = sizeof(kEndpoints_WB_MSW_ZIGBEE_v_4_official)/sizeof(kEndpoints_WB_MSW_ZIGBEE_v_4_official[0]),
};

}  // namespace zhc::devices::wirenboard
