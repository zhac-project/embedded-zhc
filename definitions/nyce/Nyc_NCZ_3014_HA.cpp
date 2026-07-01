// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nyce NCZ-3014-HA — garage door tilt sensor (z2m v26.76.0 parity).
// z2m-source: nyce.ts #NCZ-3014-HA
// Garage door tilt sensor.
// z2m extend:[m.iasZoneAlarm({zoneType:"contact",
//   zoneAttributes:["alarm_1","tamper","battery_low"]}), m.battery()].
//
// m.iasZoneAlarm zoneType "contact" maps ssIasZone (0x0500) zoneStatus bit 0
// -> `contact` and surfaces tamper (bit 2) + battery_low (bit 3); m.battery()
// = genPowerCfg (0x0001) batteryPercentageRemaining (standard /2 scale, z2m
// does NOT set dontDividePercentage here). Functionally identical to sibling
// NCZ-3011-HA: typed kFzIasContactAlarm (contact/battery_low/tamper) + kFzBattery.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nyce {
namespace {
const FzConverter* const kFz_NCZ_3014_HA[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_NCZ_3014_HA[] = { "3014" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_NCZ_3014_HA{
    .zigbee_models=kModels_NCZ_3014_HA, .zigbee_models_count=sizeof(kModels_NCZ_3014_HA)/sizeof(kModels_NCZ_3014_HA[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NCZ-3014-HA", .vendor="Nyce",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NCZ_3014_HA, .from_zigbee_count=sizeof(kFz_NCZ_3014_HA)/sizeof(kFz_NCZ_3014_HA[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nyce
