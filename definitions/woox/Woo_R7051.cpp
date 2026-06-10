// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Woox R7051 — smart siren (TS0219 standard ZCL).
// z2m-source: woox.ts #R7051.
//   fz: fz.battery, fz.ts0216_siren (ssIasWd attrs), fz.ias_alarm_only_alarm_1,
//       fz.power_source (genBasic powerSource).
//   tz: tz.warning, tz.ts0216_volume, tz.ts0216_duration (ssIasWd writeAttrs).
//
// Parity fix (z2m<->embedded-zhc pass): z2m uses fz.ias_alarm_only_alarm_1,
// which decodes ZoneStatus (attr 0x0002) bit 0 → `alarm` ONLY. The
// generated def lowered the broader kFzIasZone, which ALSO emits tamper +
// battery_low, and declared phantom `tamper` / `battery_low` exposes that
// z2m never surfaces for this device. Re-pointed at kFzIasZoneAlarmOnly
// (single `alarm` key) and dropped the two phantom exposes.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::woox {
namespace {
const FzConverter* const kFz_R7051[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZoneAlarmOnly,
    &::zhc::generic::kFzIasWdMaxDuration,
};
const TzConverter* const kTz_R7051[] = {
    &::zhc::generic::kTzWarning,
    &::zhc::generic::kTzIasWdWarningSimple,
    &::zhc::generic::kTzIasWdSquawk,
    &::zhc::generic::kTzIasWdMaxDuration,
};
constexpr const char* kModels_R7051[] = { "TS0219" };
constexpr const char* kManus_R7051[] = { "_TYZB01_ynsiasng", "_TYZB01_bwsijaty", "_TYZB01_rs7ff6o7" };

constexpr Expose kExp_R7051[] = {
    {"battery",     ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State,    "mV",    nullptr, nullptr, 0},
    {"alarm",       ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    // No tamper / battery_low: z2m's fz.ias_alarm_only_alarm_1 emits only
    // `alarm`, and the z2m R7051 exposes carry neither key.
    // Warning is a write-only command; surface a placeholder binary
    // so the WebUI can wire a "trigger" toggle. Decoded inbound side
    // remains the IAS alarm bit above.
    {"warning",     ExposeType::Binary,  Access::Set,      nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_R7051[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

// INFRA-deferred (no generic converter yet — do NOT half-build):
//   - `volume` (ssIasWd attr 0x0002 read + write, z2m fz/tz.ts0216_volume)
//     and `duration` write (z2m tz.ts0216_duration writes maxDuration);
//     needs a kFz/kTzIasWdVolumeAttr or a ZclWriteSpec table.
//   - `ac_connected` derives from genBasic.powerSource (0x0007 == 2);
//     needs a kFzPowerSource generic converter.
//   The alarm-only IAS variant is now handled by kFzIasZoneAlarmOnly.

}  // namespace

extern const PreparedDefinition kDef_R7051{
    .zigbee_models=kModels_R7051, .zigbee_models_count=sizeof(kModels_R7051)/sizeof(kModels_R7051[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_R7051, .manufacturer_names_count=sizeof(kManus_R7051)/sizeof(kManus_R7051[0]),
    .model="R7051", .vendor="Woox",
    .meta=nullptr, .exposes=kExp_R7051, .exposes_count=sizeof(kExp_R7051)/sizeof(kExp_R7051[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_R7051, .from_zigbee_count=sizeof(kFz_R7051)/sizeof(kFz_R7051[0]),
    .to_zigbee=kTz_R7051,   .to_zigbee_count=sizeof(kTz_R7051)/sizeof(kTz_R7051[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_R7051,
    .bindings_count=sizeof(kBind_R7051)/sizeof(kBind_R7051[0]),
};

}  // namespace zhc::devices::woox
