// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared CTM Lyng (Datek Wireless AS) vendor converters. CTM stuffs
// custom attributes into otherwise-standard clusters (`genOnOff`,
// `hvacThermostat`) using the DATEK_WIRELESS_AS manufacturer code
// (0x1337). Each writeable attribute gets a dedicated `TzConverter`
// that funnels through the generic `tz_zcl_write_attr` encoder with a
// `ZclWriteSpec*` describing the manuSpec write.
//
// Read-only manu-specific attribute reports decode through the
// per-cluster generic fz path today — the wire format matches a
// regular ZCL attribute report, so the standard parser will surface
// them as numeric/bool keys named after the attribute id. Devices
// that need attr-id → semantic-key remapping (e.g. `0x0408` → "mean
// _power") still need a vendor `fz_ctm_thermostat` decoder; those
// gaps are flagged TODO in `docs/CTM_PARITY.md`.
//
// z2m-source: zigbee-herdsman-converters/src/devices/ctm.ts.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::ctm {

// Datek Wireless AS — z2m manufacturer-code lookup.
constexpr std::uint16_t kCtmMfgCode = 0x1337;

// genOnOff manuSpec writes shared by mTouch_Astro, MBD-S, CTM_MBD_Dim:
//   0x4002 deviceEnabled (bool) — z2m key "device_enabled"
//   0x5001 relayState    (bool) — z2m key "state" (CTM relay overlay)
extern const TzConverter kTzCtmDeviceEnabled;
extern const TzConverter kTzCtmRelayState;

// hvacThermostat manuSpec writes used by mTouch_One:
//   0x0401 ctmLoad             (u16) — "load"          [0-3600 W]
//   0x0402 ctmDisplayText      (str) — "display_text"  [≤19 chars]
//   0x0403 ctmSensor           (e8 ) — "sensor"
//   0x0405 ctmRegulatorMode    (b  ) — "regulator_mode"
//   0x0411 ctmNightSwitching   (b  ) — "night_switching"
//   0x0412 ctmFrostGuard       (b  ) — "frost_guard"
//   0x0413 ctmChildLock        (b  ) — "child_lock"
//   0x041D ctmRegulatorSetpoint(u8 ) — "regulator_setpoint"
//   0x041E ctmRegulationMode   (e8 ) — "regulation_mode"
//   0x0422 ctmOperationMode    (e8 ) — "preset"        [off/away/sleep/home]
extern const TzConverter kTzCtmLoad;
extern const TzConverter kTzCtmDisplayText;
extern const TzConverter kTzCtmFrostGuard;
extern const TzConverter kTzCtmChildLockThermostat;
extern const TzConverter kTzCtmPreset;

// ── ssIasZone (0x0500) read-only decoders ───────────────────────────
// The generic `kFzIasZone` collapses zoneStatus bit 0 to a bare
// `alarm` key, which is WRONG for the CTM stove guards and water
// devices: z2m re-labels the bits per device. These vendor decoders
// mirror the z2m wire shape exactly and fire on the same frame set
// z2m listens to (commandStatusChangeNotification + attributeReport +
// readResponse).

// Stove guards mKomfy 2.0 (6254380) + mKomfy 2.5 (mkomfy25). z2m
// `m.iasZoneAlarm({zoneType:"generic", manufacturerZoneAttributes:[
//   {bit:0,name:"high_temperature"}, {bit:1,name:"power_cut_off"}],
//   zoneAttributes:["tamper","battery_low"]})` emits exactly:
//   bit 0 → "high_temperature", bit 1 → "power_cut_off",
//   bit 2 → "tamper", bit 3 → "battery_low".  No bare "alarm".
extern const FzConverter kFzCtmStoveGuardZone;

// AX Water Sensor + AX Valve Controller. z2m `fzLocal.ctm_water_leak
// _alarm` emits:  bit 0 → "active_water_leak", bit 1 → "water_leak",
// bit 3 → "battery_low".  Note the leak state lives on bit 1, NOT the
// bit-0 the generic `kFzIasWaterLeakAlarm` reads.
extern const FzConverter kFzCtmWaterLeak;

}  // namespace zhc::ctm
