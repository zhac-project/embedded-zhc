// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Hand-port: AduroSmart 81868 — ERIA Plug-in Siren.
// z2m-source: adurosmart.ts #81868 (v26.133.0): fingerprint modelID "Smart Siren"
// × manufacturerName "AduroSmart ERIA" / "ERIA" / "AduroSmart Eria",
// extend adurosmart.extend.siren() =
//   m.iasZoneAlarm({zoneType: "alarm", zoneAttributes: ["alarm_1"]})
//   + m.iasWarning().
//
// Mapped:
//   alarm   — zoneStatus bit 0 from a Zone Status Change Notification
//             (kFzIasGenericAlarmOnly) or a zoneStatus report / read
//             (kFzIasZoneAlarmOnly). No tamper / battery_low: alarm_1 only.
//   warning — ssIasWd startWarning (kTzWarning). z2m's `warning` is a composite
//             (mode, level, strobe, duration…); ez has no composite expose, so
//             it is a Set binary like the other sirens: true starts, false stops.
// siren() configures nothing, so there are no bindings. The old bundle's
// battery, tamper, squawk, max_duration and `alarm` start command are gone.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81868[] = { "Smart Siren" };
constexpr const char* kMfgNames_D81868[] = { "AduroSmart ERIA", "ERIA", "AduroSmart Eria" };

const FzConverter* const kFz_D81868[] = {
    &::zhc::generic::kFzIasGenericAlarmOnly,
    &::zhc::generic::kFzIasZoneAlarmOnly,
};

const TzConverter* const kTz_D81868[] = {
    &::zhc::generic::kTzWarning,
};

constexpr Expose kExposes_D81868[] = {
    { "alarm",   ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0 },
    { "warning", ExposeType::Binary, Access::Set,   nullptr, nullptr, nullptr, 0 },
};

}  // namespace

extern const PreparedDefinition kDef_D81868{
    .zigbee_models=kModels_D81868,
    .zigbee_models_count=sizeof(kModels_D81868)/sizeof(kModels_D81868[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kMfgNames_D81868,
    .manufacturer_names_count=sizeof(kMfgNames_D81868)/sizeof(kMfgNames_D81868[0]),
    .model="81868", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposes_D81868, .exposes_count=sizeof(kExposes_D81868)/sizeof(kExposes_D81868[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D81868, .from_zigbee_count=sizeof(kFz_D81868)/sizeof(kFz_D81868[0]),
    .to_zigbee=kTz_D81868, .to_zigbee_count=sizeof(kTz_D81868)/sizeof(kTz_D81868[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::adurosmart
