// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AuroraLighting AU-A1ZBDWS — hand-curated contact sensor.
// Magnetic door & window contact sensor
//
// z2m: fromZigbee=[fz.ias_contact_alarm_1, fz.battery], exposes=[contact,
// battery_low, tamper, battery]. The auto-generator wired the generic
// catch-all kFzIasZone, which publishes the un-typed `alarm` bit-0 key
// instead of the device's semantic `contact`. Swap to kFzIasContactAlarm
// (zoneType 0x0015 → `contact`, preserving tamper + battery_low) so the
// open/closed state surfaces under the key the SPA / rules expect.
//
// z2m-source: aurora_lighting.ts #AU-A1ZBDWS.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
const FzConverter* const kFz_AU_A1ZBDWS[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,  // contact + tamper + battery_low
};

constexpr const char* kModels_AU_A1ZBDWS[] = { "WindowSensor51AU" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_AU_A1ZBDWS{
    .zigbee_models=kModels_AU_A1ZBDWS, .zigbee_models_count=sizeof(kModels_AU_A1ZBDWS)/sizeof(kModels_AU_A1ZBDWS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1ZBDWS", .vendor="AuroraLighting",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AU_A1ZBDWS, .from_zigbee_count=sizeof(kFz_AU_A1ZBDWS)/sizeof(kFz_AU_A1ZBDWS[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::aurora_lighting
