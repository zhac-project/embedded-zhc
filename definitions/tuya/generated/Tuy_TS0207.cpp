// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0207 water-leak sensor.
// z2m-source: tuya.ts #TS0207.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

// Use the typed water-leak FZ so ZoneStatusChangeNotification bit 0
// surfaces as the "water_leak" expose declared below. The generic
// kFzIasZoneStatusChange would emit "alarm_1" / "alarm_2" instead and
// the UI's water_leak label would never light up. Auto-enroll lives
// at the platform layer (zigbee_mgr::zcl_maybe_respond_ias_enroll) so
// no per-device configure step is required here.
const FzConverter* const kFz_TS0207[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_TS0207[] = { "TS0207", "RB-SM0D1" };

constexpr Expose kExposes_TS0207[] = {
    { "water_leak",   ExposeType::Binary,  ::zhc::Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "tamper",       ExposeType::Binary,  ::zhc::Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "battery_low",  ExposeType::Binary,  ::zhc::Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "battery",      ExposeType::Numeric, ::zhc::Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage",      ExposeType::Numeric, ::zhc::Access::State,
      "mV", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_TS0207[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0500 },  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_TS0207{
    .zigbee_models            = kModels_TS0207,
    .zigbee_models_count      = sizeof(kModels_TS0207)/sizeof(kModels_TS0207[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "TS0207",
    .vendor                   = "Tuya",
    .meta                     = nullptr,
    .exposes                  = kExposes_TS0207,
    .exposes_count            = sizeof(kExposes_TS0207)/sizeof(kExposes_TS0207[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_TS0207,
    .from_zigbee_count        = sizeof(kFz_TS0207)/sizeof(kFz_TS0207[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_TS0207,
    .bindings_count           = sizeof(kBindings_TS0207)/sizeof(kBindings_TS0207[0]),
};

}  // namespace zhc::devices::tuya
