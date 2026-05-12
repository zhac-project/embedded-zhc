// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS1201 illuminance sensor.
// z2m-source: tuya.ts #TS1201.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

const FzConverter* const kFz_TS1201[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_TS1201[] = { "TS1201" };

constexpr Expose kExposes_TS1201[] = {
    { "illuminance", ExposeType::Numeric, ::zhc::Access::State,
      "lx", nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, ::zhc::Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State,
      "mV", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_TS1201[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0400 },  // msIlluminanceMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_TS1201{
    .zigbee_models            = kModels_TS1201,
    .zigbee_models_count      = 1,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "TS1201",
    .vendor                   = "Tuya",
    .meta                     = nullptr,
    .exposes                  = kExposes_TS1201,
    .exposes_count            = sizeof(kExposes_TS1201)/sizeof(kExposes_TS1201[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_TS1201,
    .from_zigbee_count        = sizeof(kFz_TS1201)/sizeof(kFz_TS1201[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_TS1201,
    .bindings_count           = sizeof(kBindings_TS1201)/sizeof(kBindings_TS1201[0]),
};

}  // namespace zhc::devices::tuya
