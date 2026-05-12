// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: DawonDns KB-HD100-ZB — hand-rewritten parity port.
// IOT Card holder. The card-holder reports its insert state via an
// `ssIasZone` `commandStatusChangeNotification`:
//   - bit 0 (alarm_1) → card inserted
//   - bit 3           → battery_low
// The shared `kFzIasZoneStatusChange` emits `alarm_1`/`battery_low`;
// renaming to `card` would require a typed-IAS variant in `_shared`,
// which is out of scope for this vendor sweep. We expose the underlying
// labels so the ZHC adapter still surfaces the bool transitions.
// z2m-source: dawon_dns.ts #KB-HD100-ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::dawon_dns {
namespace {

const FzConverter* const kFz_KB_HD100_ZB[] = {
    &::zhc::generic::kFzIasZoneStatusChange,
};

constexpr const char* kModels_KB_HD100_ZB[] = { "KB-HD100-ZB" };

constexpr Expose kExposes_KB_HD100_ZB[] = {
    // alarm_1 carries the card-insert bool (true = inserted).
    { "alarm_1",     ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery_low", ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_KB_HD100_ZB[] = {
    { 1, 0x0500 },  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_KB_HD100_ZB{
    .zigbee_models=kModels_KB_HD100_ZB, .zigbee_models_count=sizeof(kModels_KB_HD100_ZB)/sizeof(kModels_KB_HD100_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="KB-HD100-ZB", .vendor="DawonDns",
    .meta=nullptr, .exposes=kExposes_KB_HD100_ZB, .exposes_count=sizeof(kExposes_KB_HD100_ZB)/sizeof(kExposes_KB_HD100_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_KB_HD100_ZB, .from_zigbee_count=sizeof(kFz_KB_HD100_ZB)/sizeof(kFz_KB_HD100_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindings_KB_HD100_ZB,
    .bindings_count           = sizeof(kBindings_KB_HD100_ZB)/sizeof(kBindings_KB_HD100_ZB[0]),
};

}  // namespace zhc::devices::dawon_dns
