// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ezviz CS-T2C — open/close (contact) sensor.
// z2m ships this via m.iasZoneAlarm({zoneType:"contact",
//   zoneAttributes:["alarm_1","alarm_2","tamper","battery_low"]}) + m.battery().
// zoneType:"contact" → invertAlarmPayload, and BOTH alarm_1+alarm_2 present →
// z2m `bothAlarms` split into two semantic keys: contact_alarm_1 (=!bit0) and
// contact_alarm_2 (=!bit1). The Tier-1 auto-port wired the generic kFzIasZone,
// which emits a bare, NON-inverted `alarm` key — wrong key AND wrong polarity
// for a contact sensor. Use the typed two-key inverted contact converters.
// z2m-source: ezviz.ts #CS-T2C (lib/modernExtend.ts iasZoneAlarm).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ezviz {
namespace {
const FzConverter* const kFz_CS_T2C[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm1,
    &::zhc::generic::kFzIasContactAlarm2,
};

constexpr const char* kModels_CS_T2C[] = { "CS-T2C-A0-BG" };

}  // namespace


// Exposes follow z2m's `bothAlarms` contact split: two semantic binary keys
// (contact_alarm_1 / contact_alarm_2) plus tamper + battery_low + battery.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact_alarm_1", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"contact_alarm_2", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_CS_T2C{
    .zigbee_models=kModels_CS_T2C, .zigbee_models_count=sizeof(kModels_CS_T2C)/sizeof(kModels_CS_T2C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CS-T2C", .vendor="Ezviz",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CS_T2C, .from_zigbee_count=sizeof(kFz_CS_T2C)/sizeof(kFz_CS_T2C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ezviz
