// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 9290030674 — Hue motion outdoor sensor.
// z2m-source: philips.ts #9290030674.
//
// Hue motion sensors emit standard Zigbee battery + temperature +
// illuminance reports.  ZHC has no msOccupancySensing decoder yet, so
// the `occupancy` expose is declared but not yet wired (see
// docs/PHILIPS_PARITY.md "runtime gaps").  z2m additionally exposes
// motion_sensitivity / led_indication / occupancy_timeout via
// philips.tz.* — also not wired (Hue-specific manuSpecific configurator).
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D9290030674[] = { "SML004" };

}  // namespace

extern const PreparedDefinition kDef_D9290030674{
    .zigbee_models           = kModels_D9290030674,
    .zigbee_models_count     = sizeof(kModels_D9290030674)/sizeof(kModels_D9290030674[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "9290030674",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposesPhilipsMotionSensor,
    .exposes_count           = kExposesPhilipsMotionSensorCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzPhilipsMotionSensor,
    .from_zigbee_count       = kFzPhilipsMotionSensorCount,
    .to_zigbee               = nullptr,
    .to_zigbee_count         = 0,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsPhilipsMotionSensor,
    .bindings_count          = kBindingsPhilipsMotionSensorCount,
};

}  // namespace zhc::devices::philips
