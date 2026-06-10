// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Cleverio SA100 — smart siren (TS0219 standard ZCL).
// z2m-source: cleverio.ts #SA100.
//   fz: fz.ts0216_siren (ssIasWd attrs: duration/volume/alarm),
//       fz.ias_alarm_only_alarm_1 (ssIasZone ZoneStatus bit 0 → `alarm`),
//       fz.power_source (genBasic powerSource).
//   tz: tz.warning, tz.ts0216_volume (ssIasWd writeAttrs).
//
// Parity fix (z2m<->embedded-zhc pass): z2m decodes the alarm via
// fz.ias_alarm_only_alarm_1, which reads ssIasZone ZoneStatus (attr 0x0002)
// bit 0 → `alarm` ONLY (zoneType is the generic siren zone — bare `alarm`
// is correct here; do NOT promote to a semantic IAS type). The generated def
// lowered the broader kFzIasZone, which ALSO emits tamper + battery_low, and
// declared phantom `tamper` / `battery_low` exposes that z2m never surfaces
// for this device. Re-pointed at kFzIasZoneAlarmOnly (single `alarm` key) and
// dropped the two phantom exposes. Mirrors the sibling Woox R7051 (same
// TS0219 siren family).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::cleverio {
namespace {
const FzConverter* const kFz_SA100[] = {
    &::zhc::generic::kFzIasZoneAlarmOnly,
    &::zhc::generic::kFzIasWdMaxDuration,
};
const TzConverter* const kTz_SA100[] = {
    &::zhc::generic::kTzWarning,
    &::zhc::generic::kTzIasWdWarningSimple,
    &::zhc::generic::kTzIasWdSquawk,
    &::zhc::generic::kTzIasWdMaxDuration,
};
constexpr const char* kModels_SA100[] = { "TS0219" };
constexpr const char* kManus_SA100[] = { "_TZ3000_vdfwjopk" };

constexpr Expose kExp_SA100[] = {
    {"alarm",   ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    // No tamper / battery_low: z2m's fz.ias_alarm_only_alarm_1 emits only
    // `alarm`, and the z2m SA100 exposes carry neither key.
    // Warning is a write-only command; surface a placeholder binary so the
    // WebUI can wire a "trigger" toggle (decode side is the IAS alarm bit).
    {"warning", ExposeType::Binary, Access::Set,   nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_SA100[] = {
    {1, 0x0001},  // genPowerCfg (z2m configure binds genPowerCfg)
    {1, 0x0500},  // ssIasZone
};

// INFRA-deferred (no generic converter yet — do NOT half-build):
//   - `volume` (ssIasWd attr 0x0002 read + write, z2m fz/tz.ts0216_volume,
//     mapNumberRange 0..100 <-> 100..10); needs a kFz/kTzIasWdVolumeAttr or
//     a ZclWriteSpec table.
//   - `power_source` derives from genBasic.powerSource (attr 0x0007); needs a
//     kFzPowerSource generic converter. z2m's SA100 exposes do not list it.
//   The alarm-only IAS variant is handled by kFzIasZoneAlarmOnly; duration is
//   decoded by kFzIasWdMaxDuration / written by kTzIasWdMaxDuration.
}  // namespace

extern const PreparedDefinition kDef_SA100{
    .zigbee_models=kModels_SA100, .zigbee_models_count=sizeof(kModels_SA100)/sizeof(kModels_SA100[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SA100, .manufacturer_names_count=sizeof(kManus_SA100)/sizeof(kManus_SA100[0]),
    .model="SA100", .vendor="Cleverio",
    .meta=nullptr, .exposes=kExp_SA100, .exposes_count=sizeof(kExp_SA100)/sizeof(kExp_SA100[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SA100, .from_zigbee_count=sizeof(kFz_SA100)/sizeof(kFz_SA100[0]),
    .to_zigbee=kTz_SA100, .to_zigbee_count=sizeof(kTz_SA100)/sizeof(kTz_SA100[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SA100, .bindings_count=sizeof(kBind_SA100)/sizeof(kBind_SA100[0]),
};

}  // namespace zhc::devices::cleverio
