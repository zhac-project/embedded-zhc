// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart 81868 — hand-rewritten 2026-04-28s.
// Smart Siren — IAS Warning Device + IAS Zone + battery.
// Fingerprint: modelID="Smart Siren", manufacturerName="AduroSmart Eria".
// z2m-source: adurosmart.ts #81868.
//
// z2m bundle:
//   fromZigbee: fz.battery + fz.ias_wd + fz.ias_enroll + fz.ias_siren
//   toZigbee:   tz.warning_simple + tz.ias_max_duration + tz.warning
//   exposes:    tamper, warning, max_duration (s, get/set), alarm (binary set)
//
// Mapped:
//   kFzBattery   — genPowerCfg battery.
//   kFzIasZone   — ssIasZone status change → tamper / alarm 1/2 fields.
//   kTzWarning   — ssIasWd cmd 0x00 (startWarning) — drives the siren.
//
// 2026-04-28t post-sweep: kFzIasWdMaxDuration + kTzIasWdMaxDuration
// landed in `_generic/_shared`. `max_duration` now reads back AND
// writes via attr 0x0000 on ssIasWd. ias_enroll auto-response and
// ias_siren-style "alarm" binary set helper still pending; those
// remain documented in ADUROSMART_PARITY.md.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81868[] = { "Smart Siren" };
constexpr const char* kMfgNames_D81868[] = { "AduroSmart Eria" };

const FzConverter* const kFz_D81868[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
    &::zhc::generic::kFzIasWdMaxDuration,
};

const TzConverter* const kTz_D81868[] = {
    &::zhc::generic::kTzWarning,
    &::zhc::generic::kTzIasWdWarningSimple,
    &::zhc::generic::kTzIasWdSquawk,
    &::zhc::generic::kTzIasWdMaxDuration,
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    { "battery",      ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage",      ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0 },
    { "tamper",       ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0 },
    { "warning",      ExposeType::Binary,  Access::Set,      nullptr, nullptr, nullptr, 0 },
    { "max_duration", ExposeType::Numeric, Access::StateSet, "s",  nullptr, nullptr, 0 },
    { "alarm",        ExposeType::Binary,  Access::Set,      nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kAutoBindings[] = {
    { 1, 0x0000 },  // genBasic (z2m reads on configure)
    { 1, 0x0500 },  // ssIasZone
    { 1, 0x0502 },  // ssIasWd
    { 1, 0x0001 },  // genPowerCfg (battery)
};

extern const PreparedDefinition kDef_D81868{
    .zigbee_models=kModels_D81868,
    .zigbee_models_count=sizeof(kModels_D81868)/sizeof(kModels_D81868[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kMfgNames_D81868,
    .manufacturer_names_count=sizeof(kMfgNames_D81868)/sizeof(kMfgNames_D81868[0]),
    .model="81868", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D81868, .from_zigbee_count=sizeof(kFz_D81868)/sizeof(kFz_D81868[0]),
    .to_zigbee=kTz_D81868, .to_zigbee_count=sizeof(kTz_D81868)/sizeof(kTz_D81868[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adurosmart
