// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Adeo LDSENK08 — hand-maintained parity override.
// ENKI LEXMAN wireless smart door window sensor with vibration
//
// Graduated from generated/Ade_LDSENK08.cpp: the generated def lowered the
// generic kFzIasZone (which emits alarm_1/alarm_2) behind a dead `alarm`
// expose, so the primary contact state never reached the shadow. z2m decodes
// this via a custom fzLocal.LDSENK08 over ssIasZone status_change:
//   contact     = !(zoneStatus bit 0)     vibration   =  zoneStatus bit 1
//   tamper      =   zoneStatus bit 2       battery_low =  zoneStatus bit 3
// plus a `sensitivity` write (tzLocal.LDSENK08_sensitivity -> ssIasZone
// attr 0x0013, 0..4). Wired the generic kFzIasContactAlarm (emits `contact`
// + tamper + battery_low, same path used for every other contact port —
// e.g. the sibling ZB-DoorSensor-D0007, heiman HS1DS) + kFzBattery, and
// renamed the expose to `contact`.
// DEFERRED (no generic converter): the `vibration` bit-1 sub-flag and the
// `sensitivity` write both need a bespoke converter — the generic IAS-zone
// decoder only surfaces a single primary key + tamper/battery_low, and there
// is no generic ssIasZone attribute-write path. Tracked as INFRA.
// z2m-source: adeo.ts #LDSENK08.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {
const FzConverter* const kFz_LDSENK08[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_LDSENK08[] = { "LDSENK08" };

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

extern const PreparedDefinition kDef_LDSENK08{
    .zigbee_models=kModels_LDSENK08, .zigbee_models_count=sizeof(kModels_LDSENK08)/sizeof(kModels_LDSENK08[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LDSENK08", .vendor="Adeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LDSENK08, .from_zigbee_count=sizeof(kFz_LDSENK08)/sizeof(kFz_LDSENK08[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adeo
