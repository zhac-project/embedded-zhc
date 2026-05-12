// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-PIR-21 — hand-curated.
// Zigbee motion sensor (battery + IAS Zone occupancy alarm).
// z2m extend: m.battery(), m.iasZoneAlarm({zoneType: "occupancy",
//   zoneAttributes: ["alarm_1", "alarm_2", "tamper", "battery_low"]}).
// z2m-source: vesternet.ts #VES-ZB-PIR-21.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_PIR_21[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
};
// IAS zone sensors are read-only — no toZigbee converters.
constexpr const char* kModels_VES_ZB_PIR_21[] = { "VES-ZB-PIR-21" };

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    // m.iasZoneAlarm exposes alarm_1, alarm_2, tamper as binaries when
    // their bits are listed in zoneAttributes. The generic kFzIasZone
    // emits these keys directly from zone-status bits.
    {"alarm_1", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"alarm_2", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg → battery%
    {1, 0x0500},   // ssIasZone → zone status
};
// --- end ---

extern const PreparedDefinition kDef_VES_ZB_PIR_21{
    .zigbee_models=kModels_VES_ZB_PIR_21, .zigbee_models_count=sizeof(kModels_VES_ZB_PIR_21)/sizeof(kModels_VES_ZB_PIR_21[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-PIR-21", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VES_ZB_PIR_21, .from_zigbee_count=sizeof(kFz_VES_ZB_PIR_21)/sizeof(kFz_VES_ZB_PIR_21[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vesternet
