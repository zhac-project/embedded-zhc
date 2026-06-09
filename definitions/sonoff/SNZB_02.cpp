// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sonoff SNZB-02 / eWeLink RHK08 temperature & humidity sensor.
//
// z2m-source: sonoff.ts #SNZB-02.  z2m exposes:
//     e.battery(), e.temperature(), e.humidity(), e.battery_voltage()
//   fromZigbee: [fz.SNZB02_temperature, fz.humidity, fz.battery]
//
// The auto-generated def (definitions/sonoff/generated/Son_SNZB_02.cpp)
// lowered only `ewelinkBattery()` and so surfaced battery + voltage but
// dropped the device's primary purpose — temperature and humidity. A
// TH sensor that reports neither is broken, so this is graduated to a
// hand-maintained Tier-2 parent override.
//
// fz.SNZB02_temperature is fz.temperature (measuredValue / 100.0) plus a
// -33..100 °C sanity clamp guarding the SNZB-02's occasional garbage
// reports (z2m issue #13640). The generic kFzTemperature decoder applies
// the identical /100.0 scaling, so it is at parity for every in-range
// value; the clamp is a robustness nicety, not a semantic difference.
// Humidity (msRelativeHumidity, /100.0) maps directly to kFzHumidity.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {

constexpr const char* kModels_SNZB_02[] = {
    "TH01", "SNZB-02", "CK-TLSR8656-SS5-01(7014)",
};

constexpr Expose kExposes_SNZB_02[] = {
    { "battery",     ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, Access::State,
      "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_SNZB_02[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_SNZB_02[] = {
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
    { 1, 0x0001 },   // genPowerCfg
};

constexpr WhiteLabel kWhiteLabels_SNZB_02[] = {
    { "eWeLink", "RHK08" },
    { "eWeLink", "SNZB-02_eWeLink" },
    { "eWeLink", "CK-TLSR8656-SS5-01(7014)" },
    { "Zbeacon", "TH01" },
};

}  // namespace

extern const PreparedDefinition kDef_SNZB_02{
    .zigbee_models            = kModels_SNZB_02,
    .zigbee_models_count      = sizeof(kModels_SNZB_02)/sizeof(kModels_SNZB_02[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "SNZB-02",
    .vendor                   = "Sonoff",
    .meta                     = nullptr,
    .exposes                  = kExposes_SNZB_02,
    .exposes_count            = sizeof(kExposes_SNZB_02)/sizeof(kExposes_SNZB_02[0]),
    .white_labels             = kWhiteLabels_SNZB_02,
    .white_labels_count       = sizeof(kWhiteLabels_SNZB_02)/sizeof(kWhiteLabels_SNZB_02[0]),
    .from_zigbee              = kFz_SNZB_02,
    .from_zigbee_count        = sizeof(kFz_SNZB_02)/sizeof(kFz_SNZB_02[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_SNZB_02,
    .bindings_count           = sizeof(kBindings_SNZB_02)/sizeof(kBindings_SNZB_02[0]),
};

}  // namespace zhc::devices::sonoff
