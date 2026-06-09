// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AuroraLighting AU-A1ZBPIRS — hand-curated PIR sensor.
// AOne PIR sensor
//
// z2m: fromZigbee=[fz.ias_occupancy_alarm_1], exposes=[occupancy,
// battery_low, tamper], extend=[m.illuminance()]. The auto-generator wired
// the *generic* catch-all kFzIasZone, which publishes the un-typed `alarm`
// bit-0 key instead of the device's semantic `occupancy`, and it dropped
// the illuminance channel (m.illuminance() → `illuminance` expose + 0x0400
// binding) entirely. Result: motion never surfaced under the key a rule /
// the SPA expects, and lux was invisible.
//
// Fix: swap kFzIasZone → kFzIasMotionAlarm (zoneType 0x000D → `occupancy`,
// preserving tamper + battery_low) and add kFzIlluminance + the `illuminance`
// expose + 0x0400 binding. z2m exposes no `battery` (no fz.battery) — only
// battery_low — so we keep parity and do not add a battery numeric.
//
// z2m-source: aurora_lighting.ts #AU-A1ZBPIRS.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
const FzConverter* const kFz_AU_A1ZBPIRS[] = {
    &::zhc::generic::kFzIasMotionAlarm,  // occupancy + tamper + battery_low
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_AU_A1ZBPIRS[] = { "MotionSensor51AU" };

constexpr Expose kExp_AU_A1ZBPIRS[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_AU_A1ZBPIRS[] = {
    {1, 0x0500},  // ssIasZone
    {1, 0x0400},  // msIlluminanceMeasurement (m.illuminance())
};

}  // namespace

extern const PreparedDefinition kDef_AU_A1ZBPIRS{
    .zigbee_models=kModels_AU_A1ZBPIRS, .zigbee_models_count=sizeof(kModels_AU_A1ZBPIRS)/sizeof(kModels_AU_A1ZBPIRS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1ZBPIRS", .vendor="AuroraLighting",
    .meta=nullptr, .exposes=kExp_AU_A1ZBPIRS, .exposes_count=sizeof(kExp_AU_A1ZBPIRS)/sizeof(kExp_AU_A1ZBPIRS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AU_A1ZBPIRS, .from_zigbee_count=sizeof(kFz_AU_A1ZBPIRS)/sizeof(kFz_AU_A1ZBPIRS[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_AU_A1ZBPIRS, .bindings_count=sizeof(kBind_AU_A1ZBPIRS)/sizeof(kBind_AU_A1ZBPIRS[0]),
};

}  // namespace zhc::devices::aurora_lighting
