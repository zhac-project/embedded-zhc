// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Wirenboard WB-MSW-ZIGBEE v.3 — hand-rewritten from z2m source.
// Wall-mounted multi sensor (temperature, humidity, occupancy, CO2,
// VOC, noise + 3 on/off relays on endpoints l1/l2/l3).
// z2m-source: wirenboard.ts #WB-MSW-ZIGBEE v.3.
//
// Manufacturer code: 0x6666 (CUSTOM_SPRUT_DEVICE).
// Custom clusters used by the device:
//   sprutVoc      0x6601  (attr 0x6600 = voc           uint16)
//   sprutNoise    0x6602  (attr 0x6600 = noise         single
//                          attr 0x6601 = noiseDetected bitmap8)
//   sprutIrBlaster 0x6603 (commands only — IR remote)
//   sprutDevice   0x6680  (attr 0x6604 = isConnected   bool
//                          attr 0x6601 = UartBaudRate  u32)
// Plus manuSpec attrs piggy-backed onto standard clusters
// (msTemperatureMeasurement.sprutTemperatureOffset, etc.).
//
// The current generic kFz/kTz set only ships kFzOnOff + kFzTemperature
// + kFzHumidity + kFzIlluminance — the sprut-coded writes for
// occupancy_timeout, noise_timeout, th_heater, etc. need a future
// `wirenboard/_shared.{hpp,cpp}` bundle (Tier 2). For now the exposes
// list keeps those attributes Access::State so the shadow accepts the
// values the device pushes, and the binding list registers the
// standard clusters the device is reporting on.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::wirenboard {
namespace {

const FzConverter* const kFz_WB_MSW_ZIGBEE_v_3[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
const TzConverter* const kTz_WB_MSW_ZIGBEE_v_3[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_WB_MSW_ZIGBEE_v_3[] = { "WBMSW3" };

constexpr Expose kExposes_WB_MSW_ZIGBEE_v_3[] = {
    // Sensors (live state).
    { "temperature",       ExposeType::Numeric, Access::State,    "\xC2\xB0""C",  nullptr, nullptr, 0 },
    { "humidity",          ExposeType::Numeric, Access::State,    "%",            nullptr, nullptr, 0 },
    { "occupancy",         ExposeType::Binary,  Access::State,    nullptr,        nullptr, nullptr, 0 },
    { "occupancy_level",   ExposeType::Numeric, Access::State,    nullptr,        nullptr, nullptr, 0,
      ExposeCategory::Diagnostic },
    { "co2",               ExposeType::Numeric, Access::State,    "ppm",          nullptr, nullptr, 0 },
    { "voc",               ExposeType::Numeric, Access::State,    "ppb",          nullptr, nullptr, 0 },
    { "noise",             ExposeType::Numeric, Access::State,    "dBA",          nullptr, nullptr, 0 },
    { "noise_detected",    ExposeType::Binary,  Access::State,    nullptr,        nullptr, nullptr, 0 },

    // Three on/off relays on endpoints l1/l2/l3 (shared cluster 0x0006).
    { "state_l1",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l2",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l3",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },

    // Config (manuSpec writes — TZ not yet wired; State for shadow).
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
    { "th_heater",              ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0,
      ExposeCategory::Config },
};

constexpr BindingSpec kBindings_WB_MSW_ZIGBEE_v_3[] = {
    // Three on/off endpoints (l1/l2/l3 = ep 1/2/3 per z2m configure).
    { 1, 0x0006 },
    { 2, 0x0006 },
    { 3, 0x0006 },
    // Sensor reporting (single endpoint per z2m configure).
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
    { 1, 0x0406 },  // msOccupancySensing
    { 1, 0x040D },  // msCO2
    { 1, 0x6601 },  // sprutVoc
    { 1, 0x6602 },  // sprutNoise
};

}  // namespace

extern const PreparedDefinition kDef_WB_MSW_ZIGBEE_v_3{
    .zigbee_models=kModels_WB_MSW_ZIGBEE_v_3, .zigbee_models_count=sizeof(kModels_WB_MSW_ZIGBEE_v_3)/sizeof(kModels_WB_MSW_ZIGBEE_v_3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WB-MSW-ZIGBEE v.3", .vendor="Wirenboard",
    .meta=nullptr, .exposes=kExposes_WB_MSW_ZIGBEE_v_3,
    .exposes_count=sizeof(kExposes_WB_MSW_ZIGBEE_v_3)/sizeof(kExposes_WB_MSW_ZIGBEE_v_3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WB_MSW_ZIGBEE_v_3, .from_zigbee_count=sizeof(kFz_WB_MSW_ZIGBEE_v_3)/sizeof(kFz_WB_MSW_ZIGBEE_v_3[0]),
    .to_zigbee=kTz_WB_MSW_ZIGBEE_v_3, .to_zigbee_count=sizeof(kTz_WB_MSW_ZIGBEE_v_3)/sizeof(kTz_WB_MSW_ZIGBEE_v_3[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_WB_MSW_ZIGBEE_v_3,
    .bindings_count=sizeof(kBindings_WB_MSW_ZIGBEE_v_3)/sizeof(kBindings_WB_MSW_ZIGBEE_v_3[0]),
};

}  // namespace zhc::devices::wirenboard
