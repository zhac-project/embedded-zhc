// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Homeseer DS150ZB — IAS dead-key + phantom-expose fix.
// Door (contact) sensor.
// z2m-source: homeseer.ts #DS150ZB
//   extend: [m.battery(), m.iasZoneAlarm({zoneType:"contact",
//            zoneAttributes:["alarm_1","battery_low"]})]
//
// Bug (c) IAS dead-key: the auto-port lowered the generic kFzIasZone, which
//   emits the bare key "alarm", while z2m's iasZoneAlarm({zoneType:"contact"})
//   publishes the semantic key "contact" and inverts it (contact = !bit0,
//   invertAlarmPayload = zoneType==="contact"). With no rename layer the contact
//   state never reached the shadow. Swapped to kFzIasContactAlarm (z2m-polarity
//   inverted bit0 + tamper + battery_low).
// Bug (e) phantom exposes: the auto-port declared "alarm", "tamper" and
//   "voltage". z2m exposes none of these here — m.battery() defaults
//   voltage=false (battery % only), and the iasZoneAlarm zoneAttributes are
//   just ["alarm_1","battery_low"] (no tamper). Aligned exposes to battery +
//   contact + battery_low.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::homeseer {
namespace {
const FzConverter* const kFz_DS150ZB[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_DS150ZB[] = { "DS150ZB" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_DS150ZB{
    .zigbee_models=kModels_DS150ZB, .zigbee_models_count=sizeof(kModels_DS150ZB)/sizeof(kModels_DS150ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DS150ZB", .vendor="Homeseer",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DS150ZB, .from_zigbee_count=sizeof(kFz_DS150ZB)/sizeof(kFz_DS150ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::homeseer
