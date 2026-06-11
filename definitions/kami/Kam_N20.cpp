// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: KAMI N20 — contact + motion decode fix 2026-06-11.
// Contact sensor or motion sensor.
// z2m-source: kami.ts #N20 (fromZigbee: [fz.KAMI_contact, fz.KAMI_occupancy];
//   exposes: contact, action: ["motion"]).
//
// z2m bundle:
//   exposes: contact, action: ["motion"]
//   NO on/off, NO toZigbee.
//
// FIX: the generated port was triply wrong for this sensor:
//   1. Phantom on/off — wired generic kFzOnOff + kTzOnOff and exposed a
//      controllable `state` (+ genOnOff 0x0006 binding). The device has
//      no genOnOff cluster; z2m gives no on/off.
//   2. Wrong decoder / dead keys — wired generic kFzIasZone, which emits
//      alarm/tamper/battery_low from the standard IAS zoneStatus bitmask.
//      KAMI firmware is non-standard: z2m reads the raw frame byte 7.
//   3. Wrong exposes — should be `contact` (Binary) + `action(["motion"])`.
//
// z2m's fz.KAMI_contact (ssIasZone, raw) -> {contact: msg.data[7] === 0}
// and fz.KAMI_occupancy (msOccupancySensing, raw) -> {action: "motion"}
// when msg.data[7] === 1. Wired the vendor kFzKamiContact /
// kFzKamiOccupancy converters that do exactly that.
#include "definitions/kami/_shared.hpp"

namespace zhc::devices::kami {
namespace {
const FzConverter* const kFz_N20[] = {
    &::zhc::kami::kFzKamiContact,
    &::zhc::kami::kFzKamiOccupancy,
};
constexpr const char* kModels_N20[] = { "Z3ContactSensor" };

constexpr const char* const kActionValues[] = { "motion" };

constexpr Expose kAutoExposes[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"action",  ExposeType::Enum,   Access::State, nullptr, "Triggered action",
        kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDef_N20{
    .zigbee_models=kModels_N20, .zigbee_models_count=sizeof(kModels_N20)/sizeof(kModels_N20[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="N20", .vendor="Kami",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_N20, .from_zigbee_count=sizeof(kFz_N20)/sizeof(kFz_N20[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::kami
