// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: VSmart HS-SEDR00ZB-VNM — door/window contact via typed IAS Zone.
// Door/window sensor.
//
// z2m-source: vsmart.ts #HS-SEDR00ZB-VNM
//   extend: [m.iasZoneAlarm({zoneType: "contact",
//                            zoneAttributes: ["alarm_1", "battery_low", "tamper"],
//                            invertAlarm: true})]
//
// Parity fix: the Tier-1 auto-port wired the generic `kFzIasZone`, which
// emits a generic `"alarm"` boolean and never the `"contact"` key z2m
// advertises for a `zoneType:"contact"` device — so the SPA-/automation-
// facing expose was dead. Re-wired to the typed `kFzIasContactAlarm`
// (the same converter every other contact sensor uses — owon SPM915,
// smartthings, centralite, …), which emits `contact` + `tamper` +
// `battery_low` from the ssIasZone ZoneStatusChangeNotification.
//
// Polarity note: z2m's modernExtend sets `invertAlarmPayload = true` for
// `zoneType==="contact"`, then this device's explicit `invertAlarm:true`
// flips it back to `false` — so VSmart publishes `contact = (zoneStatus
// & 1)` (the RAW bit0, NOT the usual inverted contact). The default
// `kFzIasContactAlarm` now inverts (contact = !bit0, the z2m norm), so
// this device uses the non-inverting `kFzIasContactAlarmNI` to keep
// `contact = (zoneStatus & 1)`.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vsmart {
namespace {
const FzConverter* const kFz_HS_SEDR00ZB_VNM[] = {
    &::zhc::generic::kFzIasContactAlarmNI,
};

constexpr const char* kModels_HS_SEDR00ZB_VNM[] = { "HS-SEDR00ZB-VNM" };

constexpr Expose kAutoExposes[] = {
    {"contact",     ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

}  // namespace

extern const PreparedDefinition kDef_HS_SEDR00ZB_VNM{
    .zigbee_models=kModels_HS_SEDR00ZB_VNM, .zigbee_models_count=sizeof(kModels_HS_SEDR00ZB_VNM)/sizeof(kModels_HS_SEDR00ZB_VNM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS-SEDR00ZB-VNM", .vendor="VSmart",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS_SEDR00ZB_VNM, .from_zigbee_count=sizeof(kFz_HS_SEDR00ZB_VNM)/sizeof(kFz_HS_SEDR00ZB_VNM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vsmart
